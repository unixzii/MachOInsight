#include "core/macho/macho_data_types.h"

#include "core/macho/segment.h"

namespace macho_insight {
namespace macho {

Segment::Segment(base::AddressSpace lc_base) {
  auto lc = lc_base.As<segment_command_64>();
  name_ = (const char*) lc->segname;
  
  auto cur_section = lc_base.Skip(sizeof(segment_command_64));
  for (uint32_t i = 0; i < lc->nsects; ++i) {
    auto sect = cur_section.As<section_64>();
    
    sections_.emplace_back(Section());
    Section& section = sections_.back();
    section.name_ = (const char*) sect->sectname;
    section.vm_addr_ = sect->addr;
    section.vm_size_ = sect->size;
    section.file_offset_ = sect->offset;
    
    cur_section = cur_section.Skip(sizeof(section_64));
  }
}

}  // macho namespace
}  // macho_insight namespace
