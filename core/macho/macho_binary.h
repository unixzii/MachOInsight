#pragma once

#include <memory>
#include <string>
#include <vector>

#include "core/base/addr_space.h"
#include "core/base/object.h"
#include "core/macho/obj_types.h"
#include "core/macho/segment.h"

namespace macho_insight {

class Loader;

namespace macho {

enum class ArchType : int {
  Unknown = 0,
  X86_64 = 1,
  ARM64 = 2,
};

struct LoadDylib {
  std::string name_;
  uint32_t current_version_;
  uint32_t compatibility_version_;
};

class MachOBinary : public base::Object {
 public:
  OBJ_TYPE_DECL(ObjectType::MachOBinary)

  MachOBinary(base::AddressSpace base)
      : base_(base),
        lc_parsed_(false),
        lc_parsing_(false),
        lc_partially_parsed_allowed_(false),
        use_chained_fixups_(false) { }

  bool IsValid() const;

  ArchType arch_type() const;

  size_t LoadDylibCount();
  const LoadDylib& LoadDylibAt(size_t idx);

  size_t SegmentCount();
  const Segment& SegmentAt(size_t idx);

 private:
  friend class ChainedFixupsHelper;
  friend class macho_insight::Loader;

  void ParseLoadCommands();

  base::AddressSpace base_;

  // Load command parsing state:
  bool lc_parsed_ : 1;
  bool lc_parsing_ : 1;
  bool lc_partially_parsed_allowed_ : 1;

  bool use_chained_fixups_ : 1;

  std::vector<LoadDylib> load_dylibs_;
  std::vector<Segment> segments_;
};

}  // macho namespace
}  // macho_insight namespace
