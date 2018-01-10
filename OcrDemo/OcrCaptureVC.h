//
//  OcrCaptureVC.h
//  OcrDemo
//
//  Created by nero on 18/1/3.
//  Copyright © 2018年 Technology Co., Ltd. http://www.imwooy.com. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "OcrCaptureView.h"
#import <BaOCR/BaOCR.h>
#import <TesseractOCR/TesseractOCR.h>

@interface OcrCaptureVC : UIViewController <OcrCaptureViewDelegate>

@property (nonatomic, strong) OcrCaptureView *ocrCaptureView;

@end
