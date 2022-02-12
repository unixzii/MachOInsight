#include "variant.h"

namespace macho_insight {
namespace base {

class VariantPrivateHelper {
  const Variant& v_;

 public:
  VariantPrivateHelper(const Variant& v) : v_(v) { }

  template <typename T>
  const T& GetStorageOrDefault(int expected_type) const {
    if ((v_.data_type_ & Variant::TypeMask) == expected_type) {
      if (v_.data_type_ & Variant::FlagInlined) {
        return *(T*) v_.data_.inlined_buf;
      } else {
        return *(T*) v_.data_.ptr;
      }
    }

    static T default_val;
    return default_val;
  }

  template <typename T>
  void FreeStorage() const {
    T* storage;
    if (v_.data_type_ & Variant::FlagInlined) {
      storage = (T*) v_.data_.inlined_buf;
      storage->~T();
    } else {
      storage = (T*) v_.data_.ptr;
      delete storage;
    }
  }
};

Variant::~Variant() {
  // Compound types need to be destructed at least, and
  // free the outline storage if there is one.
  if ((data_type_ & TypeMask) == Variant::TypeString) {
    private_helper().FreeStorage<std::string>();
  } else if ((data_type_ & TypeMask) == Variant::TypeList) {
    private_helper().FreeStorage<std::vector<Variant>>();
  } else if ((data_type_ & TypeMask) == Variant::TypeMap) {
    private_helper().FreeStorage<std::unordered_map<std::string, Variant>>();
  }
}

Variant::Variant(const Variant& o) {
  *this = o;
}

Variant::Variant(Variant&& o) {
  *this = std::move(o);
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
  if (sizeof(s) <= sizeof(data_.inlined_buf)) {
    data_type_ |= Variant::FlagInlined;
    new ((void*) data_.inlined_buf) std::string(s);
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

Variant& Variant::operator=(const Variant& rhs) {
  if ((rhs.data_type_ & TypeMask) == Variant::TypeString) {
    new (this) Variant(rhs.ToString());
  } else if ((rhs.data_type_ & TypeMask) == Variant::TypeList) {
    new (this) Variant(rhs.ToList());
  } else if ((rhs.data_type_ & TypeMask) == Variant::TypeMap) {
    new (this) Variant(rhs.ToMap());
  } else {
    // Use direct copying for other plain-old-data types.
    data_ = rhs.data_;
    data_type_ = rhs.data_type_;
  }

  return *this;
}

Variant& Variant::operator=(Variant&& rhs) {
  data_ = rhs.data_;
  data_type_ = rhs.data_type_;

  // Mark invalid.
  rhs.data_type_ = 0;

  return *this;
}

int Variant::ToInt() const {
  if ((data_type_ & TypeMask) == Variant::TypeInt) {
    if (data_type_ & FlagUnsigned) {
      return (int) data_.ui;
    } else {
      return data_.i;
    }
  }
  return 0;
}

unsigned int Variant::ToUnsignedInt() const {
  if ((data_type_ & TypeMask) == Variant::TypeInt) {
    if (data_type_ & FlagUnsigned) {
      return data_.ui;
    } else {
      return (unsigned int) data_.i;
    }
  }
  return 0;
}

long long Variant::ToLongLong() const {
  if ((data_type_ & TypeMask) == Variant::TypeLongLong) {
    if (data_type_ & FlagUnsigned) {
      return (long long) data_.ull;
    } else {
      return data_.ll;
    }
  } else if ((data_type_ & TypeMask) == Variant::TypeInt) {
    return ToInt();
  }
  return 0;
}

unsigned long long Variant::ToUnsignedLongLong() const {
  if ((data_type_ & TypeMask) == Variant::TypeLongLong) {
    if (data_type_ & FlagUnsigned) {
      return data_.ull;
    } else {
      return (unsigned long long) data_.ll;
    }
  } else if ((data_type_ & TypeMask) == Variant::TypeInt) {
    return ToUnsignedInt();
  }
  return 0;
}

bool Variant::ToBool() const {
  if (data_type_ == Variant::TypeBoolean) {
    return data_.b;
  }
  return false;
}

float Variant::ToFloat() const {
  if (data_type_ == Variant::TypeFloat) {
    return data_.f;
  }
  return 0;
}

double Variant::ToDouble() const {
  if (data_type_ == Variant::TypeDouble) {
    return data_.d;
  } else if (data_type_ == Variant::TypeFloat) {
    return data_.f;
  }
  return 0;
}

const std::string& Variant::ToString() const {
  return private_helper().GetStorageOrDefault<std::string>(Variant::TypeString);
}

const std::vector<Variant>& Variant::ToList() const {
  return private_helper().GetStorageOrDefault<std::vector<Variant>>(
      Variant::TypeList);
}

const std::unordered_map<std::string, Variant>& Variant::ToMap() const {
  return private_helper()
      .GetStorageOrDefault<std::unordered_map<std::string, Variant>>(
          Variant::TypeMap);
}

VariantPrivateHelper Variant::private_helper() const {
  VariantPrivateHelper helper(*this);
  return helper;
}

}  // base namespace
}  // macho_insight namespace
