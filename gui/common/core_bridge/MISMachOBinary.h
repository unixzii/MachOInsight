#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class MISLoader;

@interface MISMachOBinary : NSObject

@property (nonatomic, readonly, assign) NSUInteger numberOfLoadDylibs;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithParentLoader:(MISLoader *)loader cxxObject:(void *)cxxObject;

@end

NS_ASSUME_NONNULL_END
