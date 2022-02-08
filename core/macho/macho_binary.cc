#include <functional>
#include <vector>

#include "core/base/addr_space.h"
#include "core/macho/chained_fixups.h"
#include "core/macho/macho_data_types.h"
#include "core/platform/byte_order.h"
#include "core/utils/logger.h"

#include "core/macho/fat_binary.h"

namespace {

using namespace macho_insight::macho;

namespace detail {

template <typename Layout, uint32_t Cmd>
struct LoadCommandParser {
  using ParserFunc = std::function<void(Layout*)>;

  LoadCommandParser(ParserFunc&& parser) : parser_(parser) { }

  bool Is(load_command* lc) const { return lc->cmd == Cmd; }

  void Parse(load_command* lc) const {
    CHECK(Is(lc)) << "Wrong input command type";
    if (parser_) {
      parser_(reinterpret_cast<Layout*>(lc));
    }
  }

 private:
  ParserFunc parser_;
};

struct LoadCommandParsingContext {
  template <typename Parser>
  void RegisterParser(Parser&& parser) {
    parsers_.push_back([parser = std::move(parser)](load_command* lc) {
      if (!parser.Is(lc)) {
        return false;
      }
      parser.Parse(lc);
      return true;
    });
  }

  void Parse(load_command* lc) {
    for (auto& parser : parsers_) {
      if (parser(lc)) {
        return;
      }
    }
  }

 private:
  std::vector<std::function<bool(load_command*)>> parsers_;
};

}  // detail namespace

using LoadDylibLoadCommandParser =
    detail::LoadCommandParser<dylib_command, LC_LOAD_DYLIB>;
using SegmentLoadCommandParser =
    detail::LoadCommandParser<segment_command_64, LC_SEGMENT_64>;
using ChainedFixupsLoadCommandParser =
    detail::LoadCommandParser<linkedit_data_command, LC_DYLD_CHAINED_FIXUPS>;

}  // anonymous namespace

namespace macho_insight {
namespace macho {

bool MachOBinary::IsValid() const {
  auto header = base_.As<mach_header_64>();
  // Use direct comparison, the byte order should be consistent.
  if (header->magic == MH_MAGIC_64) {
    return true;
  }
  return false;
}

ArchType MachOBinary::arch_type() const {
  auto header = base_.As<mach_header_64>();
  switch (header->cputype) {
    case CPU_TYPE_X86_64:
      return ArchType::X86_64;
      break;
    case CPU_TYPE_ARM64:
      return ArchType::ARM64;
    default:
      break;
  }
  return ArchType::Unknown;
}

size_t MachOBinary::LoadDylibCount() {
  ParseLoadCommands();
  return load_dylibs_.size();
}

const LoadDylib& MachOBinary::LoadDylibAt(size_t idx) {
  ParseLoadCommands();
  return load_dylibs_[idx];
}

size_t MachOBinary::SegmentCount() {
  ParseLoadCommands();
  return segments_.size();
}

const Segment& MachOBinary::SegmentAt(size_t idx) {
  ParseLoadCommands();
  return segments_[idx];
}

void MachOBinary::ParseLoadCommands() {
  if (lc_parsed_ || lc_partially_parsed_allowed_) {
    return;
  }

  if (lc_parsing_) {
    CHECK(false) << "Recursive parsing occurred, this is a bug in program";
    return;
  }
  lc_parsing_ = true;

  struct ParsingDriver {
    ParsingDriver(base::AddressSpace base,
                  size_t count,
                  detail::LoadCommandParsingContext& ctx)
        : cur_(base), count_(count), ctx_(ctx) { }

    void Run() {
      for (size_t i = 0; i < count_; ++i) {
        auto lc = cur_.As<load_command>();
        LOG(INFO) << "Parsing load command: " << lc->cmd;
        ctx_.Parse(lc);
        cur_ = cur_.Skip(lc->cmdsize);
      }
    }

   private:
    base::AddressSpace cur_;
    size_t count_;
    detail::LoadCommandParsingContext& ctx_;
  };

  detail::LoadCommandParsingContext parsing_context;
  // Register parsers.
  parsing_context.RegisterParser(
      LoadDylibLoadCommandParser([this](dylib_command* lc) {
        std::string name = ((const char*) lc) + lc->dylib.name.offset;

        // Append to the load dylib list (via an in-place fashion).
        this->load_dylibs_.emplace_back();
        LoadDylib& ent = this->load_dylibs_.back();
        ent.name_ = std::move(name);
        ent.current_version_ = lc->dylib.current_version;
        ent.compatibility_version_ = lc->dylib.compatibility_version;
      }));
  parsing_context.RegisterParser(
      SegmentLoadCommandParser([this](segment_command_64* lc) {
        this->segments_.emplace_back(base::AddressSpace(lc));
      }));
  parsing_context.RegisterParser(
      ChainedFixupsLoadCommandParser([this](linkedit_data_command* lc) {
        this->use_chained_fixups_ = true;

        // Hand off parsing works to the helper.
        ChainedFixupsHelper helper(*this, base::AddressSpace(lc));
        // Hacks: load commands are being parsed, we temporarily allow accessing
        // the data depending on them (which is safe here).
        this->lc_partially_parsed_allowed_ = true;
        helper.Parse();
        this->lc_partially_parsed_allowed_ = false;
      }));

  auto header = base_.As<mach_header_64>();
  ParsingDriver parsing_driver(base_.Skip(sizeof(mach_header_64)),
                               header->ncmds, parsing_context);

  // Start parsing!
  parsing_driver.Run();
  lc_parsed_ = true;
  lc_parsing_ = false;
}

}  // macho namespace
}  // macho_insight namespace
