#pragma once

#include "core/base/addr_space.h"
#include "core/base/variant.h"
#include "core/macho/macho_data_types.h"

namespace macho_insight {
namespace macho {

struct LoadCommandDumpHelper {
  base::AddressSpace base_;

  explicit LoadCommandDumpHelper(base::AddressSpace base) : base_(base) { }

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
