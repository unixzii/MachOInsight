#pragma once

#include <memory>
#include <vector>
#include <string>

#include "core/base/object.h"
#include "core/base/addr_space.h"
#include "core/macho/obj_types.h"
#include "core/macho/segment.h"

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
  const LoadDylib& LoadDylibAt(size_t idx);
  
  size_t SegmentCount();
  const Segment& SegmentAt(size_t idx);

 private:
  void ParseLoadCommands();
  
  base::AddressSpace base_;
  bool lc_parsed_ = false;
  
  std::vector<LoadDylib> load_dylibs_;
  std::vector<Segment> segments_;
};

}  // macho namespace
}  // macho_insight namespace
