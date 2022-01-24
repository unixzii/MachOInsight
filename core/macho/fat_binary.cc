#include <memory>

#include "core/base/addr_space.h"
#include "core/platform/byte_order.h"
#include "core/macho/macho_data_types.h"

#include "core/macho/fat_binary.h"

namespace {

using namespace macho_insight;

constexpr static uint32_t FAT_MAGIC = 0xcafebabe;
constexpr static uint32_t FAT_CIGAM = platform::SwapConst(FAT_MAGIC);

struct fat_header {
  uint32_t magic;     /* FAT_MAGIC */
  uint32_t nfat_arch; /* number of structs that follow */
};

struct fat_arch {
  uint32_t cputype;    /* cpu specifier (int) */
  uint32_t cpusubtype; /* machine specifier (int) */
  uint32_t offset;     /* file offset to this object file */
  uint32_t size;       /* size of this object file */
  uint32_t align;      /* alignment as a power of 2 */
};

}  // anonymous namespace

namespace macho_insight {
namespace macho {

bool FatBinary::IsValid() const {
  auto header = base_.As<fat_header>();
  if (header->magic == platform::SwapBigToHostConst(FAT_MAGIC)) {
    // TODO: Should we perform some additional checks?
    return true;
  }
  return false;
}

size_t FatBinary::ArchCount() const {
  auto header = base_.As<fat_header>();
  return platform::SwapBigToHostConst(header->nfat_arch);
}

ArchType FatBinary::ArchTypeAt(size_t idx) const {
  ArchType type = ArchType::Unknown;

  // Seek to the required arch segment.
  auto arch_start = base_.Skip(sizeof(fat_header)).Skip(sizeof(fat_arch) * idx);
  auto arch = arch_start.As<fat_arch>();
  switch (platform::SwapBigToHostConst(arch->cputype)) {
    case CPU_TYPE_X86_64:
      type = ArchType::X86_64;
      break;
    case CPU_TYPE_ARM64:
      type = ArchType::ARM64;
    default:
      break;
  }
  return type;
}

MachOBinary* FatBinary::MachOBinaryAt(size_t idx) const {
  // Seek to the required arch segment.
  auto arch_start = base_.Skip(sizeof(fat_header)).Skip(sizeof(fat_arch) * idx);
  auto arch = arch_start.As<fat_arch>();
  auto offset = platform::SwapBigToHostConst(arch->offset);

  std::unique_ptr<MachOBinary> sub_binary(new MachOBinary(base_.Skip(offset)));
  if (!sub_binary->IsValid()) {
    return nullptr;
  }

  // Handle off the memory management to caller.
  return sub_binary.release();
}

}  // macho namespace
}  // macho_insight namespace
