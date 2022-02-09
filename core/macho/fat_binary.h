#pragma once

#include "core/base/addr_space.h"
#include "core/base/object.h"
#include "core/macho/macho_binary.h"
#include "core/macho/obj_types.h"
#include "core/platform/mapped_file.h"

namespace macho_insight {
namespace macho {

class FatBinary : public base::Object {
 public:
  OBJ_TYPE_DECL(ObjectType::FatBinary)

  FatBinary(std::shared_ptr<platform::MappedFile>& mapped_file,
            base::AddressSpace base)
      : mapped_file_(mapped_file), base_(base) { }

  bool IsValid() const;

  size_t ArchCount() const;
  ArchType ArchTypeAt(size_t idx) const;
  std::shared_ptr<MachOBinary> MachOBinaryAt(size_t idx) const;

 private:
  std::shared_ptr<platform::MappedFile> mapped_file_;
  base::AddressSpace base_;
};

}  // macho namespace
}  // macho_insight namespace
