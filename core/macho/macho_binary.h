#pragma once

#include <memory>
#include <vector>
#include <string>

#include "core/base/object.h"
#include "core/base/addr_space.h"
#include "core/macho/obj_types.h"

namespace macho_insight {
namespace macho {

struct LoadDylib {
  std::string name_;
  uint32_t current_version_;
  uint32_t compatibility_version_;
};

class MachOBinary : public base::Object {
 public:
  OBJ_TYPE_DECL(ObjectType::MachOBinary)

  MachOBinary(base::AddressSpace base) : base_(base) { }

  bool IsValid() const;
  
  size_t LoadDylibCount();

 private:
  void ParseLoadCommands();
  
  base::AddressSpace base_;
  bool lc_parsed_ = false;
  
  std::vector<LoadDylib> load_dylibs_;
};

}  // macho namespace
}  // macho_insight namespace
