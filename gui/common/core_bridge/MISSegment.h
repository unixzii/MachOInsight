#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface MISSection : NSObject

@property (nonatomic, readonly, copy) NSString *name;

@end

@interface MISSegment : NSObject

@property (nonatomic, readonly, copy) NSString *name;
@property (nonatomic, readonly, copy) NSArray<MISSection *> *sections;

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithCxxObject:(void *)cxxObject;

@end

NS_ASSUME_NONNULL_END
