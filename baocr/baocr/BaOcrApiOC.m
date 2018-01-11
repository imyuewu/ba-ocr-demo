//
//  BaOcrApiOC.m
//  baocr
//
//  Created by nero on 2018/1/9.
//  Copyright © 2018年 Yinchengku Technology Co., Ltd. http://www.yinchengku.com. All rights reserved.
//

#import "BaOcrApiOC.h"
#import "BaOcrApi.h"
#import "BaSerialCharRcgOC.h"

@implementation BaOcrResOC

@end

@implementation BaOcrApiOC

+ (BaOcrResOC *)getBASerialOC:(NSString *)srcImagePath {
    BaOcrResOC *result = [[BaOcrResOC alloc] init];
    
    NSFileManager *defaultManager = [NSFileManager defaultManager];
    if (![defaultManager fileExistsAtPath:srcImagePath]) {
        NSLog(@"image not exists at path : %@", srcImagePath);
        result.errCode = IMAGE_FILE_NOT_EXISTS;
        return result;
    }
    
    OcrResult ocrRes = getBASerialPics([srcImagePath UTF8String]);
    result.errCode = ocrRes.errCode;
    if (ocrRes.errCode == NO_ERROR) {
        NSMutableString *serialStr = [[NSMutableString alloc] initWithCapacity:17];
        int count = 0;
        for (int i = 0; i < MAX_RET_SERIAL_IMG_COUNT; i++) {
            if (strlen(ocrRes.resData.serialImages[i]) <= 0) continue;
            NSString *text = [[BaSerialCharRcgOC sharedInstance] rcgEntiretySerial:[NSString stringWithUTF8String:ocrRes.resData.serialImages[i]]];
            text = [self eatBreakChar:text];
            if ([self validSerial:text] && count < 2) {
                [serialStr appendString:text];
                count++;
                if (count == 1) [serialStr appendString:@" "];
            }
            NSLog(@"text = %@", text);
        }
        if (count == 2) {
            result.errCode = NO_ERROR;
            result.serial = [serialStr copy];
        } else {
            result.errCode = RCG_IMAGE_SERIAL_FAIL;
        }
    }
    
    return result;
}

+ (NSString *)eatBreakChar:(NSString *)str {
    if (str.length > 8) {
        return [str substringWithRange:NSMakeRange(0, 8)];
    }
    return str;
}

+ (BOOL)validSerial:(NSString *)str {
    if ([str containsString:@" "]) return false;
    return true;
}

@end
