#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class MISLoader;
@class MISSegment;

@interface MISLoadDylib : NSObject

@property (nonatomic, readonly, copy) NSString *name;
@property (nonatomic, readonly, assign) NSUInteger currentVersion;
@property (nonatomic, readonly, assign) NSUInteger compatibilityVersion;

@end

@interface MISMachOBinary : NSObject

@property (nonatomic, readonly, assign) NSUInteger numberOfLoadDylibs;
@property (nonatomic, readonly, assign) NSUInteger numberOfSegments;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithParentLoader:(MISLoader *)loader cxxObject:(void *)cxxObject;

- (MISLoadDylib *)loadDylibAtIndex:(NSUInteger)index;

- (MISSegment *)segmentAtIndex:(NSUInteger)index;

@end

NS_ASSUME_NONNULL_END
