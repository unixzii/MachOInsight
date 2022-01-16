#include "core/utils/logger.h"

namespace macho_insight {
namespace utils {

void InitLogger() {
  FLAGS_logtostderr = 1;
  FLAGS_minloglevel = 0;
  FLAGS_stderrthreshold = 0;
  google::InitGoogleLogging("");
}

}  // utils namespace
}  // macho_insight namespace
