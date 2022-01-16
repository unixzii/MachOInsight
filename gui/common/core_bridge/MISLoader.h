#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class MISMachOBinary;

@interface MISLoader : NSObject

@property (nonatomic, readonly, assign) NSUInteger numberOfArchs;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithExecutablePath:(NSString *)path;

- (BOOL)tryLoad;

- (NSInteger)archTypeAtIndex:(NSUInteger)index;
- (MISMachOBinary *)machOBinaryAtIndex:(NSUInteger)index;

@end

NS_ASSUME_NONNULL_END
