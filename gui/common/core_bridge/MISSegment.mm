#include "core/macho/segment.h"

#import "MISSegment.h"

using namespace macho_insight;

@interface MISSection ()

- (instancetype)initWithCxxObject:(void *)cxxObject;

@end

@implementation MISSection

- (instancetype)initWithCxxObject:(void *)cxxObject {
    self = [super init];
    if (self) {
        auto section = static_cast<macho::Section *>(cxxObject);
        
        _name = [NSString stringWithUTF8String:section->Name().data()];
    }
    return self;
}

@end

@implementation MISSegment

- (instancetype)initWithCxxObject:(void *)cxxObject {
    self = [super init];
    if (self) {
        auto segment = static_cast<macho::Segment *>(cxxObject);
        
        _name = [NSString stringWithUTF8String:segment->Name().data()];
        
        auto numberOfSections = segment->SectionCount();
        NSMutableArray *sections = [NSMutableArray arrayWithCapacity:numberOfSections];
        for (size_t i = 0; i < numberOfSections; ++i) {
            auto& cxxSection = segment->SectionAt(i);
            MISSection *section = [[MISSection alloc] initWithCxxObject:(void *) &cxxSection];
            [sections addObject:section];
        }
        _sections = sections;
    }
    return self;
}

@end
