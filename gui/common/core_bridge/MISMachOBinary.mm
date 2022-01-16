#include <memory>

#include "core/macho/macho_binary.h"

#import "MISMachOBinary.h"

using namespace macho_insight;

@implementation MISMachOBinary {
    MISLoader *_parent;
    std::unique_ptr<macho::MachOBinary> _cxxObject;
}

- (instancetype)initWithParentLoader:(MISLoader *)loader cxxObject:(void *)cxxObject {
    self = [super init];
    if (self) {
        _parent = loader;
        _cxxObject.reset(reinterpret_cast<macho::MachOBinary *>(cxxObject));
    }
    return self;
}

- (NSUInteger)numberOfLoadDylibs {
    if (!_cxxObject) {
        return 0;
    }
    return _cxxObject->LoadDylibCount();
}

@end
