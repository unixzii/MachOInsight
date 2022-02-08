#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, MISArchType) {
    MISArchTypeUnknown = 0,
    MISArchTypeX86_64 = 1,
    MISArchTypeARM64 = 2,
};

@class MISMachOBinary;

@interface MISLoader : NSObject

@property (nonatomic, readonly, assign) NSUInteger numberOfArchs;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithExecutablePath:(NSString *)path;

- (BOOL)tryLoad;

- (MISArchType)archTypeAtIndex:(NSUInteger)index;
- (MISMachOBinary *)machOBinaryAtIndex:(NSUInteger)index;

@end

NS_ASSUME_NONNULL_END
