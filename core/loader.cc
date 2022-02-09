#include "core/loader.h"
#include "core/macho/fat_binary.h"
#include "core/platform/mapped_file.h"
#include "core/utils/logger.h"

namespace macho_insight {

bool Loader::TryLoad() {
  if (loaded_) {
    return true;
  }

  if (path_.empty()) {
    LOG(ERROR) << "Failed to load, the path is empty";
    return false;
  }

  auto mapped_file = std::shared_ptr<platform::MappedFile>(
      platform::MappedFile::Create(path_));
  if (!mapped_file) {
    LOG(ERROR) << "Failed to load, cannot create mapped file";
    return false;
  }

  auto file_base = mapped_file->Start();
  auto maybe_fat_binary = std::make_shared<macho::FatBinary>(mapped_file, file_base);
  if (maybe_fat_binary->IsValid()) {
    content_ = maybe_fat_binary;
    LOG(INFO) << "Fat binary loaded, with " << maybe_fat_binary->ArchCount()
              << " arch(s)";
  } else {
    LOG(INFO) << "Not a fat binary";
    auto maybe_macho_binary = std::make_shared<macho::MachOBinary>(mapped_file, file_base);
    if (maybe_macho_binary->IsValid()) {
      content_ = maybe_macho_binary;
      LOG(INFO) << "Mach-O binary loaded";
    } else {
      LOG(INFO) << "Not a Mach-O binary";
      return false;
    }
  }

  mapped_file_ = mapped_file;
  loaded_ = true;
  return true;
}

size_t Loader::ArchCount() const {
  if (content_.valueless_by_exception()) {
    return 0;
  }

  if (!IsFatBinary()) {
    return 1;
  }
  return std::get<FatBinaryPtr>(content_)->ArchCount();
}

macho::ArchType Loader::ArchTypeAt(size_t idx) const {
  if (content_.valueless_by_exception()) {
    return macho::ArchType::Unknown;
  }

  if (!IsFatBinary()) {
    if (idx > 0) {
      return macho::ArchType::Unknown;
    }
    return std::get<MachOBinaryPtr>(content_)->arch_type();
  }
  return std::get<FatBinaryPtr>(content_)->ArchTypeAt(idx);
}

std::shared_ptr<macho::MachOBinary> Loader::MachOBinaryAt(size_t idx) const {
  if (content_.valueless_by_exception()) {
    return nullptr;
  }

  if (!IsFatBinary()) {
    return std::get<MachOBinaryPtr>(content_);
  }
  return std::get<FatBinaryPtr>(content_)->MachOBinaryAt(idx);
}

}  // macho_insight namespace
