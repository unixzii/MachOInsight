#pragma once

#include <memory>
#include <string>
#include <variant>

#include "core/macho/fat_binary.h"
#include "core/platform/mapped_file.h"

namespace macho_insight {

class Loader {
 public:
  Loader(const std::string& path) : path_(path) { }
  Loader(std::string&& path) : path_(std::move(path)) { }

  bool TryLoad();

  inline bool IsFatBinary() const { return content_.index() == 0; }
  size_t ArchCount() const;
  macho::ArchType ArchTypeAt(size_t idx) const;
  std::shared_ptr<macho::MachOBinary> MachOBinaryAt(size_t idx) const;

 private:
  using FatBinaryPtr = std::shared_ptr<macho::FatBinary>;
  using MachOBinaryPtr = std::shared_ptr<macho::MachOBinary>;

  std::string path_;
  std::shared_ptr<platform::MappedFile> mapped_file_;
  std::variant<FatBinaryPtr, MachOBinaryPtr> content_;
  bool loaded_ = false;
};

}  // macho_insight namespace
