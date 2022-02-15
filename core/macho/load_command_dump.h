#pragma once

#include "core/base/addr_space.h"
#include "core/base/variant.h"
#include "core/macho/macho_data_types.h"

namespace macho_insight {
namespace macho {

struct LoadCommandDumpHelper {
  base::AddressSpace base_;

  explicit LoadCommandDumpHelper(base::AddressSpace base) : base_(base) { }

  base::Variant DumpSegment(segment_command_64* lc) const {
    std::unordered_map<std::string, base::Variant> root;
    root["name"] = std::string("LC_SEGMENT_64 (") + lc->segname + ")";

    std::vector<base::Variant> fields;
    fields.push_back(MakeField("Command", &lc->cmd, "LC_SEGMENT_64"));
    fields.push_back(MakeField("Command Size", &lc->cmdsize, lc->cmdsize));
    fields.push_back(MakeField("Segment Name", &lc->segname, lc->segname));
    fields.push_back(MakeField("VM Address", &lc->vmaddr, lc->vmaddr));
    fields.push_back(MakeField("VM Size", &lc->vmsize, lc->vmsize));
    fields.push_back(
        MakeField("Maximum VM Protection", &lc->maxprot, lc->maxprot));
    fields.push_back(
        MakeField("Initial VM Protection", &lc->initprot, lc->initprot));
    fields.push_back(MakeField("Number of Sections", &lc->nsects, lc->nsects));
    fields.push_back(MakeField("Flags", &lc->flags, lc->flags));

    root["fields"] = fields;

    // Process sections.
    std::vector<base::Variant> children;
    children.reserve(lc->nsects);
    for (size_t isect = 0; isect < lc->nsects; ++isect) {
      auto sect = (section_64*) (lc + 1) + isect;

      std::unordered_map<std::string, base::Variant> child_root;
      child_root["name"] = std::string("Section (") + sect->sectname + ")";

      std::vector<base::Variant> fields;
      fields.push_back(MakeField("Address", &sect->addr, sect->addr));
      fields.push_back(MakeField("Size", &sect->size, sect->size));
      fields.push_back(MakeField("File Offset", &sect->offset, sect->offset));
      fields.push_back(MakeField("Alignment", &sect->align, sect->align));
      fields.push_back(
          MakeField("File Offset of Relocations", &sect->reloff, sect->reloff));
      fields.push_back(
          MakeField("Number of Relocations", &sect->nreloc, sect->nreloc));
      fields.push_back(MakeField("Flags", &sect->flags, sect->flags));
      fields.push_back(
          MakeField("Reserved1", &sect->reserved1, sect->reserved1));
      fields.push_back(
          MakeField("Reserved2", &sect->reserved2, sect->reserved2));
      fields.push_back(
          MakeField("Reserved3", &sect->reserved3, sect->reserved3));

      child_root["fields"] = fields;
      children.emplace_back(child_root);
    }

    root["children"] = children;

    return root;
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

    std::unordered_map<std::string, base::Variant> root;
    root["name"] = "LC_LOAD_DYLIB (" + dylib_last_component + ")";

    std::vector<base::Variant> fields;
    fields.push_back(MakeField("Command", &lc->cmd, "LC_LOAD_DYLIB"));
    fields.push_back(MakeField("Command Size", &lc->cmdsize, lc->cmdsize));
    fields.push_back(MakeField("Name Str Offset", &lc->dylib.name.offset,
                               lc->dylib.name.offset));
    fields.push_back(
        MakeField("Timestamp", &lc->dylib.timestamp, lc->dylib.timestamp));
    fields.push_back(MakeField("Current Version", &lc->dylib.current_version,
                               lc->dylib.current_version));
    fields.push_back(MakeField("Compatibility Version",
                               &lc->dylib.compatibility_version,
                               lc->dylib.compatibility_version));
    fields.push_back(MakeField(
        "Name", ((const char*) lc) + lc->dylib.name.offset, dylib_name, true));

    root["fields"] = fields;

    return root;
  }

 private:
  template <typename T>
  base::Variant MakeField(const std::string& name,
                          T* addr,
                          base::Variant value,
                          bool virtual_field = false) const {
    std::unordered_map<std::string, base::Variant> field;
    field["name"] = name;
    field["addr"] =
        (unsigned long long) ((uintptr_t) addr - (uintptr_t) base_.As<char>());
    field["size"] = (unsigned long long) sizeof(T);
    field["value"] = value;
    field["virtual"] = virtual_field;
    return field;
  }
};

}  // macho namespace
}  // macho_insight namespace
