#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace macho_insight {
namespace base {

class Variant final {
 public:
  ~Variant();

  Variant(int i);
  Variant(unsigned int ui);
  Variant(long long ll);
  Variant(unsigned long long ull);
  Variant(bool b);
  Variant(float f);
  Variant(double d);

  Variant(const std::string& s);

  Variant(const std::vector<Variant>& list);
  Variant(const std::unordered_map<std::string, Variant>& map);

 private:
  union Data {
    int i;
    unsigned int ui;
    long long ll;
    unsigned long long ull;
    bool b;
    float f;
    double d;
    void* inlined_buf[3];
    void* ptr;
  } data_;

  enum DataType : int {
    TypeInt = 1,
    TypeLongLong = 2,
    TypeBoolean = 3,
    TypeFloat = 4,
    TypeDouble = 5,
    TypeString = 6,
    TypeList = 7,
    TypeMap = 8,

    FlagInlined = 1 << 6,
    FlagUnsigned = 2 << 6,

    TypeMask = 0x3f,
  };
  int data_type_;
};

}  // base namespace
}  // macho_insight namespace