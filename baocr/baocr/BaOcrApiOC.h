//
//  BaOcrApiOC.h
//  baocr
//
//  Created by nero on 2018/1/9.
//  Copyright © 2018年 Yinchengku Technology Co., Ltd. http://www.yinchengku.com. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BaType.h"

@interface BaOcrResOC : NSObject

@property (nonatomic, assign) int errCode;
@property (nonatomic, assign) OcrResultType resType;
@property (nonatomic, assign) Boolean isBaImage;
@property (nonatomic, copy) NSString *serial;
@property (nonatomic, strong) NSDate *expireDate;

@end

@interface BaOcrApiOC : NSObject

+ (BaOcrResOC *)getBASerialOC:(NSString *)srcImagePath;

@end
