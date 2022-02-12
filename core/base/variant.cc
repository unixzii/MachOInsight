#include "variant.h"

namespace macho_insight {
namespace base {

Variant::~Variant() {
  if (data_type_ == Variant::TypeString) {
    // Need to free the storage of the outlined string.
    delete (std::string*) data_.ptr;
  }
  // Other compound types need to be destructed at least.
  else if ((data_type_ & TypeMask) == Variant::TypeList) {
    std::vector<Variant>* list;
    if (data_type_ & FlagInlined) {
      list = (decltype(list)) data_.inlined_buf;
      list->~vector();
    } else {
      list = (decltype(list)) data_.ptr;
      delete list;
    }
  } else if ((data_type_ & TypeMask) == Variant::TypeMap) {
    std::unordered_map<std::string, Variant>* map;
    if (data_type_ & FlagInlined) {
      map = (decltype(map)) data_.inlined_buf;
      map->~unordered_map();
    } else {
      map = (decltype(map)) data_.ptr;
      delete map;
    }
  }
}

Variant::Variant(int i) : data_type_(Variant::TypeInt) {
  data_.i = i;
}

Variant::Variant(unsigned int ui)
    : data_type_(Variant::TypeInt | Variant::FlagUnsigned) {
  data_.ui = ui;
}

Variant::Variant(long long ll) : data_type_(Variant::TypeLongLong) {
  data_.ll = ll;
}

Variant::Variant(unsigned long long ull)
    : data_type_(Variant::TypeLongLong | Variant::FlagUnsigned) {
  data_.ull = ull;
}

Variant::Variant(bool b) : data_type_(Variant::TypeBoolean) {
  data_.b = b;
}

Variant::Variant(float f) : data_type_(Variant::TypeFloat) {
  data_.f = f;
}

Variant::Variant(double d) : data_type_(Variant::TypeDouble) {
  data_.d = d;
}

Variant::Variant(const std::string& s) : data_type_(Variant::TypeString) {
  if (s.size() <= sizeof(data_.inlined_buf)) {
    data_type_ |= Variant::FlagInlined;
    std::copy(s.begin(), s.end(), (char*) data_.inlined_buf);
  } else {
    data_.ptr = new std::string(s);
  }
}

Variant::Variant(const std::vector<Variant>& list)
    : data_type_(Variant::TypeList) {
  if (sizeof(list) <= sizeof(data_.inlined_buf)) {
    data_type_ |= Variant::FlagInlined;
    new ((void*) data_.inlined_buf) std::vector<Variant>(list);
  } else {
    data_.ptr = new std::vector<Variant>(list);
  }
}

Variant::Variant(const std::unordered_map<std::string, Variant>& map)
    : data_type_(Variant::TypeMap) {
  if (sizeof(map) <= sizeof(data_.inlined_buf)) {
    data_type_ |= Variant::FlagInlined;
    new ((void*) data_.inlined_buf)
        std::unordered_map<std::string, Variant>(map);
  } else {
    data_.ptr = new std::unordered_map<std::string, Variant>(map);
  }
}

}  // base namespace
}  // macho_insight namespace
