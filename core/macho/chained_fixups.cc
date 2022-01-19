#include <functional>

#include "core/macho/macho_binary.h"
#include "core/macho/macho_data_types.h"

#include "core/macho/chained_fixups.h"

namespace {

using namespace macho_insight;
using namespace macho_insight::macho;

namespace detail {

void ForEachFixupsInSegment(
    base::AddressSpace image_base,
    const dyld_chained_starts_in_segment* starts,
    std::function<void(base::AddressSpace fixup_loc)> handler) {
  for (uint32_t page_idx = 0; page_idx < starts->page_count; ++page_idx) {
    uint16_t offset_in_page = starts->page_start[page_idx];
    if (offset_in_page == DYLD_CHAINED_PTR_START_NONE)
      continue;
    if (offset_in_page & DYLD_CHAINED_PTR_START_MULTI) {
      // 32-bit chains which may need multiple starts per page
      uint32_t overflow_index = offset_in_page & ~DYLD_CHAINED_PTR_START_MULTI;
      bool chain_end = false;
      while (!chain_end) {
        chain_end =
            (starts->page_start[overflow_index] & DYLD_CHAINED_PTR_START_LAST);
        offset_in_page =
            (starts->page_start[overflow_index] & ~DYLD_CHAINED_PTR_START_LAST);
        auto page_start = image_base.Skip(starts->segment_offset +
                                          page_idx * starts->page_size);
        auto chain = page_start.Skip(offset_in_page);

        handler(chain);

        ++overflow_index;
      }
    } else {
      // one chain per page
      auto page_start = image_base.Skip(starts->segment_offset +
                                        page_idx * starts->page_size);
      auto chain = page_start.Skip(offset_in_page);

      handler(chain);
    }
  }
}

}  // detail namespace

}  // anonymous namespace

namespace macho_insight {
namespace macho {

void ChainedFixupsHelper::Parse() {
  auto lc = lc_base_.As<linkedit_data_command>();

  auto header = image_.base_.Skip(lc->dataoff);
  auto starts_in_image =
      header.Skip(header.As<dyld_chained_fixups_header>()->starts_offset)
          .As<dyld_chained_starts_in_image>();

  for (uint32_t seg_idx = 0; seg_idx < starts_in_image->seg_count; ++seg_idx) {
    auto seg_info_offset = starts_in_image->seg_info_offset[seg_idx];
    if (seg_info_offset == 0)
      continue;

    const dyld_chained_starts_in_segment* seg_info =
        (dyld_chained_starts_in_segment*) ((uint8_t*) starts_in_image +
                                           seg_info_offset);

    detail::ForEachFixupsInSegment(
        image_.base_, seg_info,
        [this](auto fixup_loc) { this->ParseFixup(fixup_loc); });
  }
}

// base::AddressSpace ChainedFixupsHelper::GetLinkeditSegment() const {
//  size_t seg_count = image_.SegmentCount();
//  for (size_t idx = 0; idx < seg_count; ++idx) {
//    const Segment& seg = image_.SegmentAt(idx);
//    if (seg.Name() == "__LINKEDIT") {
//      return image_.base_.Skip(seg.file_offset());
//    }
//  }
//  return base::AddressSpace(nullptr);
//}

void ChainedFixupsHelper::ParseFixup(base::AddressSpace fixup_loc) {
  // TODO: Implement me.
}

}  // macho namespace
}  // macho_insight namespace
