#pragma once

#include "core/platform/byte_order.h"

namespace macho_insight {
namespace macho {

constexpr static uint32_t MH_MAGIC_64 = 0xfeedfacf;
constexpr static uint32_t MH_CIGAM_64 = platform::SwapConst(MH_MAGIC_64);

constexpr static uint32_t CPU_ARCH_MASK =
    0xff000000; /* mask for architecture bits */
constexpr static uint32_t CPU_ARCH_ABI64 = 0x01000000; /* 64 bit ABI */
constexpr static uint32_t CPU_ARCH_ABI64_32 =
    0x02000000; /* ABI for 64-bit hardware with 32-bit types; LP32 */

constexpr static uint32_t CPU_TYPE_X86 = ((uint32_t) 7);
constexpr static uint32_t CPU_TYPE_X86_64 = (CPU_TYPE_X86 | CPU_ARCH_ABI64);
constexpr static uint32_t CPU_TYPE_ARM = ((uint32_t) 12);
constexpr static uint32_t CPU_TYPE_ARM64 = (CPU_TYPE_ARM | CPU_ARCH_ABI64);

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

constexpr static uint32_t LC_REQ_DYLD = 0x80000000;

constexpr static uint32_t LC_LOAD_DYLIB = 0xc;
constexpr static uint32_t LC_SEGMENT_64 = 0x19;
constexpr static uint32_t LC_DYLD_INFO_ONLY = 0x22 | LC_REQ_DYLD;
constexpr static uint32_t LC_RPATH = 0x1c | LC_REQ_DYLD;

constexpr static uint32_t LC_DYLD_CHAINED_FIXUPS = 0x80000034;

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

struct dyld_info_command {
  uint32_t cmd;             /* LC_DYLD_INFO or LC_DYLD_INFO_ONLY */
  uint32_t cmdsize;         /* sizeof(struct dyld_info_command) */
  uint32_t rebase_off;      /* file offset to rebase info  */
  uint32_t rebase_size;     /* size of rebase info   */
  uint32_t bind_off;        /* file offset to binding info   */
  uint32_t bind_size;       /* size of binding info  */
  uint32_t weak_bind_off;   /* file offset to weak binding info   */
  uint32_t weak_bind_size;  /* size of weak binding info  */
  uint32_t lazy_bind_off;   /* file offset to lazy binding info */
  uint32_t lazy_bind_size;  /* size of lazy binding infs */
  uint32_t export_off;      /* file offset to lazy binding info */
  uint32_t export_size;     /* size of lazy binding infs */
};

struct linkedit_data_command {
  uint32_t cmd;       /* LC_CODE_SIGNATURE, LC_SEGMENT_SPLIT_INFO,
                         LC_FUNCTION_STARTS, LC_DATA_IN_CODE,
                         LC_DYLIB_CODE_SIGN_DRS,
                         LC_LINKER_OPTIMIZATION_HINT,
                         LC_DYLD_EXPORTS_TRIE, or
                         LC_DYLD_CHAINED_FIXUPS. */
  uint32_t cmdsize;   /* sizeof(struct linkedit_data_command) */
  uint32_t dataoff;   /* file offset of data in __LINKEDIT segment */
  uint32_t datasize;  /* file size of data in __LINKEDIT segment  */
};

struct dyld_chained_fixups_header {
  uint32_t fixups_version;  // 0
  uint32_t starts_offset;   // offset of dyld_chained_starts_in_image in chain_data
  uint32_t imports_offset;  // offset of imports table in chain_data
  uint32_t symbols_offset;  // offset of symbol strings in chain_data
  uint32_t imports_count;   // number of imported symbol names
  uint32_t imports_format;  // DYLD_CHAINED_IMPORT*
  uint32_t symbols_format;  // 0 => uncompressed, 1 => zlib compressed
};

struct dyld_chained_starts_in_image {
  uint32_t seg_count;
  uint32_t seg_info_offset[1];  // each entry is offset into this struct for that segment
                                // followed by pool of dyld_chain_starts_in_segment data
};

struct dyld_chained_starts_in_segment {
  uint32_t size;               // size of this (amount kernel needs to copy)
  uint16_t page_size;          // 0x1000 or 0x4000
  uint16_t pointer_format;     // DYLD_CHAINED_PTR_*
  uint64_t segment_offset;     // offset in memory to start of segment
  uint32_t max_valid_pointer;  // for 32-bit OS, any value beyond this is not a pointer
  uint16_t page_count;         // how many pages are in array
  uint16_t page_start[1];      // each entry is offset in each page of first element in chain
                               // or DYLD_CHAINED_PTR_START_NONE if no fixups on page
};

enum {
  DYLD_CHAINED_PTR_START_NONE   = 0xFFFF, // used in page_start[] to denote a page with no fixups
  DYLD_CHAINED_PTR_START_MULTI  = 0x8000, // used in page_start[] to denote a page which has multiple starts
  DYLD_CHAINED_PTR_START_LAST   = 0x8000, // used in chain_starts[] to denote last start in list for page
};

struct rpath_command {
  uint32_t cmd;       /* LC_RPATH */
  uint32_t cmdsize;   /* includes string */
  union lc_str path;  /* path to add to run path */
};

}  // macho namespace
}  // macho_insight namespace
