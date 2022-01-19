#include "capstone/capstone.h"

#include "core/disasm/engine.h"

void __TestStub() {
  // TODO: Remove this soon, currently keep to test compilation.
  csh handle;
  cs_open(CS_ARCH_ARM64, CS_MODE_64, &handle);
}
