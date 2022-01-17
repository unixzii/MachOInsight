#pragma once

namespace macho_insight {
namespace macho {

static const int kObjectTypeBitShift = 1;

enum class ObjectType : int {
  FatBinary   = 1 << kObjectTypeBitShift,
  MachOBinary = 2 << kObjectTypeBitShift,
  Segment     = 3 << kObjectTypeBitShift,
  Section     = 4 << kObjectTypeBitShift,
};

}  // macho namespace
}  // macho_insight namespace
