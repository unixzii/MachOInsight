#pragma once

#include <stddef.h>
#include <cstdint>

namespace macho_insight {
namespace base {

class AddressSpace {
 public:
  AddressSpace(void* base) : base_(base) { }

  template<typename T>
  T LoadAs() const {
    T* ptr = As<T>();
    return *ptr;
  }

  template<typename T>
  T* As() const {
    return reinterpret_cast<T*>(base_);
  }
  
  AddressSpace Skip(size_t bytes) const {
    return AddressSpace((void*) ((uintptr_t) base_ + bytes));
  }

 private:
  void* base_;
};

}  // base namespace
}  // macho_insight namespace
