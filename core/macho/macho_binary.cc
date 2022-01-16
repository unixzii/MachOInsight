#include <functional>
#include <vector>

#include "core/base/addr_space.h"
#include "core/platform/byte_order.h"
#include "core/utils/logger.h"

#include "core/macho/fat_binary.h"

namespace {

using namespace macho_insight;

constexpr static uint32_t MH_MAGIC_64 = 0xfeedfacf;
constexpr static uint32_t MH_CIGAM_64 = platform::SwapConst(MH_MAGIC_64);

struct mach_header_64 {
  uint32_t magic;       /* mach magic number identifier */
  uint32_t cputype;     /* cpu specifier */
  uint32_t cpusubtype;  /* machine specifier */
  uint32_t filetype;    /* type of file */
  uint32_t ncmds;       /* number of load commands */
  uint32_t sizeofcmds;  /* the size of all the load commands */
  uint32_t flags;       /* flags */
  uint32_t reserved;    /* reserved */
};

constexpr static uint32_t LC_LOAD_DYLIB = 0xc;

union lc_str {
  uint32_t offset;  /* offset to the string */
#ifndef __LP64__
  char* ptr;        /* pointer to the string */
#endif
};

struct dylib {
  union lc_str  name;              /* library's path name */
  uint32_t timestamp;              /* library's build time stamp */
  uint32_t current_version;        /* library's current version number */
  uint32_t compatibility_version;  /* library's compatibility vers number*/
};

struct load_command {
  uint32_t cmd;      /* type of load command */
  uint32_t cmdsize;  /* total size of command in bytes */
};

struct dylib_command {
  uint32_t cmd;        /* LC_ID_DYLIB, LC_LOAD_{,WEAK_}DYLIB,
                          LC_REEXPORT_DYLIB */
  uint32_t cmdsize;    /* includes pathname string */
  struct dylib dylib;  /* the library identification */
};

namespace detail {

template<typename Layout, uint32_t Cmd>
struct LoadCommandParser {
  using ParserFunc = std::function<void(Layout*)>;
  
  LoadCommandParser(ParserFunc&& parser) : parser_(parser) { }
  
  bool Is(load_command* lc) const {
    return lc->cmd == Cmd;
  }
  
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
  template<typename Parser>
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

using LoadDylibLoadCommandParser = detail::LoadCommandParser<dylib_command, LC_LOAD_DYLIB>;

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

size_t MachOBinary::LoadDylibCount() {
  ParseLoadCommands();
  return load_dylibs_.size();
}

void MachOBinary::ParseLoadCommands() {
  if (lc_parsed_) {
    return;
  }
  
  struct ParsingDriver {
    ParsingDriver(
      base::AddressSpace base,
      size_t count,
      detail::LoadCommandParsingContext& ctx
   ) : cur_(base), count_(count), ctx_(ctx) { }
    
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
  parsing_context.RegisterParser(LoadDylibLoadCommandParser([this](dylib_command* lc) {
    std::string name = ((char*) lc) + lc->dylib.name.offset;
    
    // Append to the load dylib list (via an in-place fashion).
    this->load_dylibs_.emplace_back();
    LoadDylib& ent = this->load_dylibs_.back();
    ent.name_ = std::move(name);
    ent.current_version_ = lc->dylib.current_version;
    ent.compatibility_version_ = lc->dylib.compatibility_version;
  }));
  
  auto header = base_.As<mach_header_64>();
  ParsingDriver parsing_driver(base_.Skip(sizeof(mach_header_64)),
                               header->ncmds, parsing_context);
  
  // Start parsing!
  parsing_driver.Run();
  lc_parsed_ = true;
}

}  // macho namespace
}  // macho_insight namespace
