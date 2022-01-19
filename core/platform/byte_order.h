#pragma once

#include <cstdint>

namespace macho_insight {
namespace platform {

template <typename T>
struct SwapConstTy;

template <>
struct SwapConstTy<uint16_t> {
  constexpr uint16_t operator()(uint16_t x) const {
    return (uint16_t)((((uint16_t)(x)&0xff00U) >> 8) |
                      (((uint16_t)(x)&0x00ffU) << 8));
  }
};

template <>
struct SwapConstTy<uint32_t> {
  constexpr uint32_t operator()(uint32_t x) const {
    return (uint32_t)((((uint32_t)(x)&0xff000000U) >> 24) |
                      (((uint32_t)(x)&0x00ff0000U) >> 8) |
                      (((uint32_t)(x)&0x0000ff00U) << 8) |
                      (((uint32_t)(x)&0x000000ffU) << 24));
  }
};

template <>
struct SwapConstTy<uint64_t> {
  uint64_t operator()(uint64_t x) const {
    return (uint64_t)((((uint64_t)(x)&0xff00000000000000ULL) >> 56) |
                      (((uint64_t)(x)&0x00ff000000000000ULL) >> 40) |
                      (((uint64_t)(x)&0x0000ff0000000000ULL) >> 24) |
                      (((uint64_t)(x)&0x000000ff00000000ULL) >> 8) |
                      (((uint64_t)(x)&0x00000000ff000000ULL) << 8) |
                      (((uint64_t)(x)&0x0000000000ff0000ULL) << 24) |
                      (((uint64_t)(x)&0x000000000000ff00ULL) << 40) |
                      (((uint64_t)(x)&0x00000000000000ffULL) << 56));
  }
};

template <typename T>
constexpr T SwapConst(T x) {
  return SwapConstTy<T>()(x);
}

template <typename T>
constexpr T SwapBigToHostConst(T x) {
#if defined(__BIG_ENDIAN__)
  return x;
#elif defined(__LITTLE_ENDIAN__)
  return SwapConstTy<T>()(x);
#else
#error Unknown endianess.
#endif
}

}  // platform namespace
}  // macho_insight namespace
