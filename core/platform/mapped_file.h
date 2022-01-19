#pragma once

#include <string>

#include "core/base/addr_space.h"

namespace macho_insight {
namespace platform {

class MappedFile final {
 public:
  static MappedFile* Create(const std::string& path);

  ~MappedFile();

  MappedFile() = delete;
  MappedFile(const MappedFile& o) = delete;

  base::AddressSpace Start() const { return base::AddressSpace(buf_); }

 private:
  MappedFile(void* buf, int fd, size_t len) : buf_(buf), fd_(fd), len_(len) { }

  void* buf_;
  int fd_ = -1;
  size_t len_ = 0;
};

}  // platform namespace
}  // macho_insight namespace
