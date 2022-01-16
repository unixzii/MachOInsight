#pragma once

#include "core/base/object.h"
#include "core/base/addr_space.h"
#include "core/macho/obj_types.h"
#include "core/macho/macho_binary.h"

namespace macho_insight {
namespace macho {

enum class ArchType : int {
  Unknown = 0,
  X86_64 = 1,
  ARM64 = 2,
};

class FatBinary : public base::Object {
 public:
  OBJ_TYPE_DECL(ObjectType::FatBinary)

  FatBinary(base::AddressSpace base) : base_(base) { }

  bool IsValid() const;
  
  size_t ArchCount() const;
  ArchType ArchTypeAt(size_t idx) const;
  MachOBinary* MachOBinaryAt(size_t idx) const;

 private:
  base::AddressSpace base_;
};

}  // macho namespace
}  // macho_insight namespace
