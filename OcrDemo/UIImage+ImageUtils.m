//
//  UIImage+ImageUtils.m
//  OcrDemo
//
//  Created by 雷祥祥 on 2018/1/5.
//  Copyright © 2018年 Yinchengku Technology Co., Ltd. http://www.yinchengku.com. All rights reserved.
//

#import "UIImage+ImageUtils.h"

@implementation UIImage (ImageUtils)

- (UIImage *)croppedImage:(CGRect)bounds {
    CGImageRef imageRef = CGImageCreateWithImageInRect([self CGImage], bounds);
    UIImage *croppedImage = [UIImage imageWithCGImage:imageRef];
    CGImageRelease(imageRef);
    return croppedImage;
}
    
@end
