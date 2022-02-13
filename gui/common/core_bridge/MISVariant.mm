#include "core/base/variant.h"

#import "MISVariant.h"

using namespace macho_insight;

@implementation MISVariant {
    std::shared_ptr<base::Variant> _cxxObject;
}

- (instancetype)initWithCxxObject:(void **)cxxObject {
    self = [super init];
    if (self) {
        auto cxxObjectPtr = reinterpret_cast<std::shared_ptr<base::Variant> *>(cxxObject);
        _cxxObject = std::move(*cxxObjectPtr);
    }
    return self;
}

- (long long)toLongLong {
    return _cxxObject->ToLongLong();
}

- (unsigned long long)toUnsignedLongLong {
    return _cxxObject->ToUnsignedLongLong();
}

- (NSString *)toString {
    return [NSString stringWithUTF8String:_cxxObject->ToString().data()];
}

- (NSArray<MISVariant *> *)toArray {
    const auto &list = _cxxObject->ToList();
    NSMutableArray *array = [NSMutableArray arrayWithCapacity:list.size()];
    for (const auto &element : list) {
        auto copy = std::make_shared<base::Variant>(element);
        [array addObject:[[MISVariant alloc] initWithCxxObject:(void **) &copy]];
    }
    return [array copy];
}

- (NSDictionary<NSString *,MISVariant *> *)toDictionary {
    const auto &map = _cxxObject->ToMap();
    NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:map.size()];
    for (const auto &pair : map) {
        auto key = [NSString stringWithUTF8String:pair.first.data()];
        auto valueCopy = std::make_shared<base::Variant>(pair.second);
        dict[key] = [[MISVariant alloc] initWithCxxObject:(void **) &valueCopy];
    }
    return [dict copy];
}

- (NSString *)description {
    auto &variant = *_cxxObject;
    auto dataType = variant.data_type();
    switch (dataType & base::Variant::TypeMask) {
        case base::Variant::TypeString:
            return [NSString stringWithUTF8String:variant.ToString().data()];
        case base::Variant::TypeInt: {
            if (dataType & base::Variant::FlagUnsigned) {
                return @(variant.ToUnsignedInt()).description;
            }
            return @(variant.ToInt()).description;
        }
        case base::Variant::TypeLongLong: {
            if (dataType & base::Variant::FlagUnsigned) {
                return @(variant.ToUnsignedLongLong()).description;
            }
            return @(variant.ToLongLong()).description;
        }
        case base::Variant::TypeFloat:
            return @(variant.ToFloat()).description;
        case base::Variant::TypeBoolean:
            return @(variant.ToBool()).description;
        case base::Variant::TypeList: {
            auto count = variant.ToList().size();
            return [NSString stringWithFormat:@"List (%@ elements)", @(count)];
        }
        case base::Variant::TypeMap: {
            auto count = variant.ToMap().size();
            return [NSString stringWithFormat:@"Map (%@ elements)", @(count)];
        }
        default:
            break;
    }
    return [super description];
}

@end
