#pragma once

#define OBJ_TYPE_DECL(x) \
constexpr static int TypeId = (int) (x); \
int Type() const override { \
  return (int) (x); \
}

namespace macho_insight {
namespace base {

class Object {
 public:
  virtual ~Object() = default;
  
  virtual int Type() const = 0;
  
  template<typename T>
  T* As() const {
    if (Type() != T::TypeId) {
      return nullptr;
    }
    return static_cast<T*>(this);
  }
};

}  // base namespace
}  // macho_insight namespace
