#pragma once

#include "core/platform/byte_order.h"

namespace macho_insight {
namespace macho {

constexpr static uint32_t MH_MAGIC_64 = 0xfeedfacf;
constexpr static uint32_t MH_CIGAM_64 = platform::SwapConst(MH_MAGIC_64);

struct mach_header_64 {
  uint32_t magic;       /* mach magic number identifier */
  uint32_t cputype;     /* cpu specifier */
  uint32_t cpusubtype;  /* machine specifier */
  uint32_t filetype;    /* type of file */
  uint32_t ncmds;       /* number of load commands */
  uint32_t sizeofcmds;  /* the size of all the load commands */
  uint32_t flags;       /* flags */
  uint32_t reserved;    /* reserved */
};

constexpr static uint32_t LC_LOAD_DYLIB = 0xc;
constexpr static uint32_t LC_SEGMENT_64 = 0x19;

union lc_str {
  uint32_t offset;  /* offset to the string */
#ifndef __LP64__
  char* ptr;        /* pointer to the string */
#endif
};

struct dylib {
  union lc_str  name;              /* library's path name */
  uint32_t timestamp;              /* library's build time stamp */
  uint32_t current_version;        /* library's current version number */
  uint32_t compatibility_version;  /* library's compatibility vers number*/
};

struct load_command {
  uint32_t cmd;      /* type of load command */
  uint32_t cmdsize;  /* total size of command in bytes */
};

struct dylib_command {
  uint32_t cmd;        /* LC_ID_DYLIB, LC_LOAD_{,WEAK_}DYLIB,
                          LC_REEXPORT_DYLIB */
  uint32_t cmdsize;    /* includes pathname string */
  struct dylib dylib;  /* the library identification */
};

struct segment_command_64 { /* for 64-bit architectures */
  uint32_t cmd;       /* LC_SEGMENT_64 */
  uint32_t cmdsize;   /* includes sizeof section_64 structs */
  char segname[16];   /* segment name */
  uint64_t vmaddr;    /* memory address of this segment */
  uint64_t vmsize;    /* memory size of this segment */
  uint64_t fileoff;   /* file offset of this segment */
  uint64_t filesize;  /* amount to map from the file */
  uint32_t maxprot;   /* maximum VM protection */
  uint32_t initprot;  /* initial VM protection */
  uint32_t nsects;    /* number of sections in segment */
  uint32_t flags;     /* flags */
};

struct section_64 { /* for 64-bit architectures */
  char sectname[16];   /* name of this section */
  char segname[16];    /* segment this section goes in */
  uint64_t addr;       /* memory address of this section */
  uint64_t size;       /* size in bytes of this section */
  uint32_t offset;     /* file offset of this section */
  uint32_t align;      /* section alignment (power of 2) */
  uint32_t reloff;     /* file offset of relocation entries */
  uint32_t nreloc;     /* number of relocation entries */
  uint32_t flags;      /* flags (section type and attributes)*/
  uint32_t reserved1;  /* reserved (for offset or index) */
  uint32_t reserved2;  /* reserved (for count or sizeof) */
  uint32_t reserved3;  /* reserved */
};

}  // macho namespace
}  // macho_insight namespace
