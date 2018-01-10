//
//  OcrCaptureManager.h
//  OcrDemo
//
//  Created by nero on 18/1/3.
//  Copyright © 2018年 Technology Co., Ltd. http://www.imwooy.com. All rights reserved.
//

#import <UIKit/UIKit.h>

typedef void (^ CaptureBlock) (NSString *imagePath);

@interface OcrCaptureManager : NSObject

@property (nonatomic, strong) dispatch_queue_t sessionQueue;
@property (nonatomic, strong) AVCaptureSession *session;
@property (nonatomic, strong) AVCaptureDevice *device;
@property (nonatomic, strong) AVCaptureDeviceInput *input;
@property (nonatomic, strong) AVCaptureStillImageOutput *output;
@property (nonatomic, strong) AVCaptureVideoPreviewLayer *previewLayer;
@property (nonatomic, assign) CGSize sizeRatio;

- (void)initCameraWithRect:(CGRect)rect;
- (void)takePic:(CaptureBlock)captureBlock;

@end
