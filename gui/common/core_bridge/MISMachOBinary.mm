#include <memory>

#include "core/macho/macho_binary.h"

#import "MISMachOBinary.h"
#import "MISSegment.h"

using namespace macho_insight;

@interface MISLoadDylib ()

@property (nonatomic, readwrite, copy) NSString *name;
@property (nonatomic, readwrite, assign) NSUInteger currentVersion;
@property (nonatomic, readwrite, assign) NSUInteger compatibilityVersion;

@end

@implementation MISLoadDylib
@end

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

- (NSUInteger)numberOfSegments {
    if (!_cxxObject) {
        return 0;
    }
    return _cxxObject->SegmentCount();
}

- (MISLoadDylib *)loadDylibAtIndex:(NSUInteger)index {
    if (!_cxxObject) {
        return nil;
    }
    
    auto &cxxObject = _cxxObject->LoadDylibAt(index);
    
    MISLoadDylib *object = [[MISLoadDylib alloc] init];
    object.name = [NSString stringWithUTF8String:cxxObject.name_.data()];
    object.currentVersion = cxxObject.current_version_;
    object.compatibilityVersion = cxxObject.compatibility_version_;
    
    return object;
}

- (MISSegment *)segmentAtIndex:(NSUInteger)index {
    if (!_cxxObject) {
        return nil;
    }
    
    auto &cxxObject = _cxxObject->SegmentAt(index);
    
    return [[MISSegment alloc] initWithCxxObject:(void *) &cxxObject];
}

@end
