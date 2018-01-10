//
//  BaSerialCharRcgOC.h
//  baocr
//
//  Created by nero on 2018/1/9.
//  Copyright © 2018年 Yinchengku Technology Co., Ltd. http://www.yinchengku.com. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BaSerialCharRcgOC : NSObject


+ (BaSerialCharRcgOC *)sharedInstance;
- (NSString *)rcgEntiretySerial:(NSString *)imagePath;
- (NSString *)rcgSingleSerialChar:(NSString *)imagePath;

@end
