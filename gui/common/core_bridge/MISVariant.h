#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface MISVariant : NSObject

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithCxxObject:(void * _Nullable * _Nonnull)cxxObject;

- (long long)toLongLong;
- (unsigned long long)toUnsignedLongLong;

- (NSString * _Nullable)toString;

- (NSArray<MISVariant *> * _Nullable)toArray;
- (NSDictionary<NSString *, MISVariant *> * _Nullable)toDictionary;

@end

NS_ASSUME_NONNULL_END
