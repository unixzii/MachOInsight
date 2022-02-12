#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace macho_insight {
namespace base {

class VariantPrivateHelper;

class Variant final {
 public:
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

  ~Variant();

  Variant(const Variant& o);
  Variant(Variant&& o);

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

  Variant& operator=(const Variant& rhs);
  Variant& operator=(Variant&& rhs);

  inline DataType data_type() const { return (DataType) data_type_; }

  int ToInt() const;
  unsigned int ToUnsignedInt() const;
  long long ToLongLong() const;
  unsigned long long ToUnsignedLongLong() const;
  bool ToBool() const;
  float ToFloat() const;
  double ToDouble() const;
  const std::string& ToString() const;
  const std::vector<Variant>& ToList() const;
  const std::unordered_map<std::string, Variant>& ToMap() const;

 private:
  friend class VariantPrivateHelper;

  VariantPrivateHelper private_helper() const;

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

  int data_type_;
};

}  // base namespace
}  // macho_insight namespace