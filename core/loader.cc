#include "core/loader.h"
#include "core/platform/mapped_file.h"
#include "core/utils/logger.h"
#include "core/macho/fat_binary.h"

namespace macho_insight {

bool Loader::TryLoad() {
  if (loaded_) {
    return true;
  }

  if (path_.empty()) {
    LOG(ERROR) << "Failed to load, the path is empty";
    return false;
  }

  auto mapped_file = std::shared_ptr<platform::MappedFile>(platform::MappedFile::Create(path_));
  if (!mapped_file) {
    LOG(ERROR) << "Failed to load, cannot create mapped file";
    return false;
  }

  auto file_base = mapped_file->Start();
  auto maybe_fat_binary = std::make_shared<macho::FatBinary>(file_base);
  if (!maybe_fat_binary->IsValid()) {
    LOG(INFO) << "Not a fat binary";
    return false;
  }
  
  LOG(INFO) << "Fat binary loaded, with " << maybe_fat_binary->ArchCount() << " arch(s)";

  mapped_file_ = mapped_file;
  fat_binary_ = maybe_fat_binary;
  loaded_ = true;
  return true;
}

size_t Loader::ArchCount() const {
  if (!fat_binary_) {
    return 0;
  }
  return fat_binary_->ArchCount();
}

macho::ArchType Loader::ArchTypeAt(size_t idx) const {
  if (!fat_binary_) {
    return macho::ArchType::Unknown;
  }
  return fat_binary_->ArchTypeAt(idx);
}

macho::MachOBinary* Loader::MachOBinaryAt(size_t idx) const {
  if (!fat_binary_) {
    return nullptr;
  }
  return fat_binary_->MachOBinaryAt(idx);
}

}  // macho_insight namespace
