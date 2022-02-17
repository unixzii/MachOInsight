#pragma once

#include <functional>

#include "core/base/addr_space.h"
#include "core/base/variant.h"
#include "core/macho/macho_data_types.h"

namespace macho_insight {
namespace macho {

namespace detail {

struct LCDumpTreeBuilder {
  LCDumpTreeBuilder(std::string&& name, base::AddressSpace base) : base_(base) {
    root_["name"] = name;
  }

  template <typename T>
  void AddField(const std::string& name,
                T* addr,
                base::Variant value,
                bool virtual_field = false) {
    std::unordered_map<std::string, base::Variant> field;
    field["name"] = name;
    field["addr"] =
        (unsigned long long) ((uintptr_t) addr - (uintptr_t) base_.As<char>());
    field["size"] = (unsigned long long) sizeof(T);
    field["value"] = value;
    field["virtual"] = virtual_field;

    fields_.emplace_back(field);
  }

  void AddChildWithBuilder(std::string&& name,
                           const std::function<void(LCDumpTreeBuilder&)>& fn) {
    LCDumpTreeBuilder child_builder(std::move(name), base_);
    fn(child_builder);

    children_.emplace_back(child_builder.Build());
  }

  base::Variant Build() {
    root_["fields"] = fields_;
    if (!children_.empty()) {
      root_["children"] = children_;
    }
    return root_;
  }

 private:
  std::unordered_map<std::string, base::Variant> root_;
  std::vector<base::Variant> fields_;
  std::vector<base::Variant> children_;

  base::AddressSpace base_;
};

}  // detail namespace

struct LoadCommandDumpHelper {
  explicit LoadCommandDumpHelper(base::AddressSpace base) : base_(base) { }

  base::Variant DumpSegment(segment_command_64* lc) const {
    detail::LCDumpTreeBuilder builder(
        std::string("LC_SEGMENT_64 (") + lc->segname + ")", base_);

    builder.AddField("Command", &lc->cmd, "LC_SEGMENT_64");
    builder.AddField("Command Size", &lc->cmdsize, lc->cmdsize);
    builder.AddField("Segment Name", &lc->segname, lc->segname);
    builder.AddField("VM Address", &lc->vmaddr,
                     (unsigned long long) lc->vmaddr);
    builder.AddField("VM Size", &lc->vmsize, (unsigned long long) lc->vmsize);
    builder.AddField("File Offset", &lc->fileoff,
                     (unsigned long long) lc->fileoff);
    builder.AddField("File Size", &lc->filesize,
                     (unsigned long long) lc->filesize);
    builder.AddField("Maximum VM Protection", &lc->maxprot, lc->maxprot);
    builder.AddField("Initial VM Protection", &lc->initprot, lc->initprot);
    builder.AddField("Number of Sections", &lc->nsects, lc->nsects);
    builder.AddField("Flags", &lc->flags, lc->flags);

    // Process sections.
    for (size_t isect = 0; isect < lc->nsects; ++isect) {
      auto sect = (section_64*) (lc + 1) + isect;

      builder.AddChildWithBuilder(
          std::string("Section (") + sect->sectname + ")",
          [sect](detail::LCDumpTreeBuilder& builder) {
            builder.AddField("Address", &sect->addr,
                             (unsigned long long) sect->addr);
            builder.AddField("Size", &sect->size,
                             (unsigned long long) sect->size);
            builder.AddField("File Offset", &sect->offset, sect->offset);
            builder.AddField("Alignment", &sect->align, sect->align);
            builder.AddField("File Offset of Relocations", &sect->reloff,
                             sect->reloff);
            builder.AddField("Number of Relocations", &sect->nreloc,
                             sect->nreloc);
            builder.AddField("Flags", &sect->flags, sect->flags);
            builder.AddField("Reserved1", &sect->reserved1, sect->reserved1);
            builder.AddField("Reserved2", &sect->reserved2, sect->reserved2);
            builder.AddField("Reserved3", &sect->reserved3, sect->reserved3);
          });
    }

    return builder.Build();
  }

  base::Variant DumpLoadDylib(dylib_command* lc) const {
    std::string dylib_name = ((const char*) lc) + lc->dylib.name.offset;
    std::string dylib_last_component = ([&dylib_name]() {
      auto last_path_sep_pos = dylib_name.find_last_of('/');
      if (last_path_sep_pos != std::string::npos) {
        return dylib_name.substr(last_path_sep_pos + 1);
      }
      return dylib_name;
    })();

    detail::LCDumpTreeBuilder builder(
        "LC_LOAD_DYLIB (" + dylib_last_component + ")", base_);

    builder.AddField("Command", &lc->cmd, "LC_LOAD_DYLIB");
    builder.AddField("Command Size", &lc->cmdsize, lc->cmdsize);
    builder.AddField("Name Str Offset", &lc->dylib.name.offset,
                     lc->dylib.name.offset);
    builder.AddField("Timestamp", &lc->dylib.timestamp, lc->dylib.timestamp);
    builder.AddField("Current Version", &lc->dylib.current_version,
                     lc->dylib.current_version);
    builder.AddField("Compatibility Version", &lc->dylib.compatibility_version,
                     lc->dylib.compatibility_version);
    builder.AddField("Name", ((const char*) lc) + lc->dylib.name.offset,
                     dylib_name, true);

    return builder.Build();
  }

 private:
  base::AddressSpace base_;
};

}  // macho namespace
}  // macho_insight namespace
