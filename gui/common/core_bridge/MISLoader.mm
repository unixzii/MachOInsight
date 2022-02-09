#include <memory>

#include "core/loader.h"

#import "MISLoader.h"
#import "MISMachOBinary.h"

using namespace macho_insight;

@implementation MISLoader {
    std::shared_ptr<Loader> _cxxLoader;
}

- (instancetype)initWithExecutablePath:(NSString *)path {
    self = [super init];
    if (self) {
        auto cxxLoader = new Loader(std::string(path.UTF8String));
        _cxxLoader.reset(cxxLoader);
    }
    return self;
}

- (BOOL)tryLoad {
    return _cxxLoader->TryLoad();
}

- (NSUInteger)numberOfArchs {
    return _cxxLoader->ArchCount();
}

- (MISArchType)archTypeAtIndex:(NSUInteger)index {
    return (MISArchType) _cxxLoader->ArchTypeAt(index);
}

- (id)machOBinaryAtIndex:(NSUInteger)index {
    auto cxxBinary = _cxxLoader->MachOBinaryAt(index);
    return [[MISMachOBinary alloc] initWithCxxObject:(void **) &cxxBinary];
}

@end
