#pragma once

#include "core/base/addr_space.h"

namespace macho_insight {
namespace macho {

class MachOBinary;

class ChainedFixupsHelper {
 public:
  ChainedFixupsHelper(MachOBinary& image, base::AddressSpace lc_base)
    : image_(image), lc_base_(lc_base) { }

  void Parse();

 private:
//  base::AddressSpace GetLinkeditSegment() const;
  
  void ParseFixup(base::AddressSpace fixup_loc);

  MachOBinary& image_;
  base::AddressSpace lc_base_;
};

}  // macho namespace
}  // macho_insight namespace
