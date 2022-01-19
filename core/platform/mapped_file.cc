#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "core/platform/mapped_file.h"
#include "core/utils/logger.h"

namespace macho_insight {
namespace platform {

MappedFile::~MappedFile() {
  if (munmap(buf_, len_)) {
    LOG(ERROR) << "Failed to munmap, memory leak may happen";
  }
  close(fd_);
}

MappedFile* MappedFile::Create(const std::string& path) {
  int fd = open(path.data(), O_RDONLY);
  if (fd < 0) {
    LOG(ERROR) << "Failed to open file: " << path;
    return nullptr;
  }

  struct stat file_stat;
  fstat(fd, &file_stat);

  auto buf =
      mmap(nullptr, file_stat.st_size, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0);
  if (buf == MAP_FAILED) {
    LOG(ERROR) << "Failed to mmap, errno: " << errno;
    return nullptr;
  }

  return new MappedFile(buf, fd, file_stat.st_size);
}

}  // platform namespace
}  // macho_insight namespace
