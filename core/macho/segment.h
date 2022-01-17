#pragma once

#include <vector>
#include <string>

#include "core/base/object.h"
#include "core/base/addr_space.h"
#include "core/macho/obj_types.h"

namespace macho_insight {
namespace macho {

class Section : public base::Object {
 public:
  OBJ_TYPE_DECL(ObjectType::Section)
  
  const std::string& Name() const {
    return name_;
  }
  
 private:
  friend class Segment;
  
  // Can only be constructed by `Segment`.
  explicit Section() = default;
  
  std::string name_;
  uintptr_t vm_addr_;
  size_t vm_size_;
  size_t file_offset_;
};

class Segment : public base::Object {
 public:
  OBJ_TYPE_DECL(ObjectType::Segment)
  
  Segment(base::AddressSpace lc_base);
  
  const std::string& Name() const {
    return name_;
  }
  
  size_t SectionCount() const {
    return sections_.size();
  }
  const Section& SectionAt(size_t idx) const {
    return sections_[idx];
  }

 private:
  std::string name_;
  std::vector<Section> sections_;
};

}  // macho namespace
}  // macho_insight namespace
