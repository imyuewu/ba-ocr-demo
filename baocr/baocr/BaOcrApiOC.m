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

@implementation BaOcrApiOC

+ (NSString *)getBASerialOC:(NSString *)srcImagePath {
    NSMutableString *serialStr = [[NSMutableString alloc] initWithCapacity:17];
    int size = 6;
    int len = 512;
    char images[size][len];
    for (int i = 0; i < 6; i++) {
        images[i][0] = '\0';
    }
    NSFileManager *defaultManager = [NSFileManager defaultManager];
    if ([defaultManager fileExistsAtPath:srcImagePath]) {
        NSLog(@"file exists at path : %@", srcImagePath);
    }
    getBASerialPics([srcImagePath
                     
                     
                     UTF8String], size, len, images);
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (strlen(images[i]) <= 0) continue;
        NSString *text = [[BaSerialCharRcgOC sharedInstance] rcgEntiretySerial:[NSString stringWithUTF8String:images[i]]];
        text = [self eatBreakChar:text];
        if ([self validSerial:text] && count < 2) {
            [serialStr appendString:text];
            count++;
            if (count == 1) [serialStr appendString:@" "];
        }
        NSLog(@"text = %@", text);
    }
    return [serialStr copy];
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
