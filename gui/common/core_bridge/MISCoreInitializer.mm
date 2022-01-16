#include "core/utils/logger.h"

#import "MISCoreInitializer.h"

@implementation MISCoreInitializer

+ (void)load {
    macho_insight::utils::InitLogger();
}

@end
