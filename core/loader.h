#pragma once

#include <memory>
#include <string>

#include "core/macho/fat_binary.h"
#include "core/platform/mapped_file.h"

namespace macho_insight {

class Loader {
 public:
  Loader(const std::string& path) : path_(path) { }
  Loader(std::string&& path) : path_(std::move(path)) { }

  bool TryLoad();

  size_t ArchCount() const;
  macho::ArchType ArchTypeAt(size_t idx) const;
  macho::MachOBinary* MachOBinaryAt(size_t idx) const;

 private:
  std::string path_;
  std::shared_ptr<platform::MappedFile> mapped_file_;
  std::shared_ptr<macho::FatBinary> fat_binary_;
  bool loaded_ = false;
};

}  // macho_insight namespace
