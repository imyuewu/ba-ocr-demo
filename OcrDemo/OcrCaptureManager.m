//
//  OcrCaptureManager.m
//  OcrDemo
//
//  Created by nero on 18/1/3.
//  Copyright © 2018年 Technology Co., Ltd. http://www.imwooy.com. All rights reserved.
//

#import "OcrCaptureManager.h"

#define kSessionQueueLabel "OcrSessionQueue"
@interface OcrCaptureManager ()
    
@end

@implementation OcrCaptureManager

- (void)dealloc {
//    dispatch_async(self.sessionQueue, ^{
//
//    });
    NSLog(@"************ Ocr Capture Manager Dead ************** ");
    if ([self.session isRunning]) {
        [self.session stopRunning];
    }
    self.previewLayer = nil;
    self.output = nil;
    self.input = nil;
    self.device = nil;
    self.session = nil;
}

- (void)initCameraWithRect:(CGRect)rect {
//    dispatch_async(self.sessionQueue, ^{
//
//    });
    if ([self.session canAddInput:self.input]) {
        [self.session addInput:self.input];
    } else {
        NSLog(@"Capture session add input fail!");
        return ;
    }
    
    if ([self.session canAddOutput:self.output]) {
        [self.session addOutput:self.output];
    } else {
        NSLog(@"Capture session add output fail!");
        return ;
    }
    
    self.previewLayer.frame = rect;
    [self.session startRunning];
}

- (void)takePic:(CaptureBlock)captureBlock {
    AVCaptureConnection *connection = [self.output connectionWithMediaType:AVMediaTypeVideo];
    
    if (connection.isVideoOrientationSupported) {
        connection.videoOrientation = [self currentVideoOrientation];
    }
    
    id handler = ^(CMSampleBufferRef imageDataSampleBuffer, NSError *error) {
        if (imageDataSampleBuffer != NULL) {
            NSData *imageData = [AVCaptureStillImageOutput jpegStillImageNSDataRepresentation:imageDataSampleBuffer];
            UIImage *image = [[UIImage alloc] initWithData:imageData];
//            image = [self resizeImage:image];
            NSLog(@"originImage:%@", [NSValue valueWithCGSize:image.size]);
            CGSize srcSize = image.size;
            CGSize dstSize = CGSizeMake(srcSize.width * self.sizeRatio.width, srcSize.height * self.sizeRatio.height);
            CGRect dstRect = CGRectMake((srcSize.width - dstSize.width) / 2, (srcSize.height - dstSize.height) / 2, dstSize.width, dstSize.height);
            UIImage *dstImg = [image croppedImage:dstRect];
            NSString *path = [FileUtils genImageSavePath];
            [UIImageJPEGRepresentation(dstImg, 1.0f) writeToFile:path atomically:NO];
            if (captureBlock) {
                captureBlock(path);
            }
        }
    };
    
    // Capture still image
    [self.output captureStillImageAsynchronouslyFromConnection:connection completionHandler:handler];

}
    
- (UIImage *)resizeImage:(UIImage *)srcImage {
    CGSize srcSize = srcImage.size;
    float diviceRatio = SCREEN_WIDTH / SCREEN_HEIGHT;
    float imageRatio = srcSize.height / srcSize.width;
    CGSize dstSize;
    CGRect dstRect = CGRectMake(0, 0, srcSize.width, srcSize.height);
    if (diviceRatio > imageRatio) {
        dstSize = CGSizeMake(srcSize.width * (1 - (diviceRatio - imageRatio)), srcSize.height);
        dstRect = CGRectMake(srcSize.width / 2.0 - dstSize.width / 2.0, 0, dstSize.width, dstSize.height);
    } else if (diviceRatio < imageRatio) {
        dstSize = CGSizeMake(srcSize.width, srcSize.height * (1 - (imageRatio - diviceRatio)));
        dstRect = CGRectMake(0, srcSize.height / 2.0 - dstSize.height / 2.0, dstSize.width, dstSize.height);
    }
    return [srcImage croppedImage:dstRect];
}

    
- (dispatch_queue_t)sessionQueue {
    if (!_sessionQueue) {
        _sessionQueue = dispatch_queue_create(kSessionQueueLabel, DISPATCH_QUEUE_SERIAL);
    }
    return _sessionQueue;
}

- (AVCaptureSession *)session {
    if (!_session) {
        _session = [[AVCaptureSession alloc] init];
        if ([_session canSetSessionPreset:AVCaptureSessionPreset1920x1080]) {
            [_session setSessionPreset:AVCaptureSessionPreset1920x1080];
        }
    }
    return _session;
}

- (AVCaptureDevice *)device {
    if (!_device) {
        _device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    }
    return _device;
}

- (AVCaptureDeviceInput *)input {
    if (!_input) {
        NSError *error = nil;
        _input = [[AVCaptureDeviceInput alloc] initWithDevice:self.device error:&error];
        if (error) {
            NSLog(@"Get device input fail, reason：%@", error.localizedDescription);
        }
    }
    return _input;
}

- (AVCaptureStillImageOutput *)output {
    if (!_output) {
        _output = [[AVCaptureStillImageOutput alloc] init];
        NSDictionary *outputSettings = @{AVVideoCodecKey:AVVideoCodecJPEG};//输出jpeg
        _output.outputSettings = outputSettings;
    }
    return _output;
}

- (AVCaptureVideoPreviewLayer *)previewLayer {
    if (!_previewLayer) {
        _previewLayer = [[AVCaptureVideoPreviewLayer alloc] initWithSession:self.session];
        _previewLayer.videoGravity = AVLayerVideoGravityResizeAspectFill;
        
        //        UIBezierPath* path = [UIBezierPath bezierPathWithArcCenter:CGPointMake(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2) radius:PREVIEW_EDGE_LEN / 2 startAngle:0 endAngle:2*M_PI clockwise:YES];
        //        CAShapeLayer* shape = [CAShapeLayer layer];
        //        shape.path = path.CGPath;
        //
        //        preview.mask = shape;
        //        self.previewLayer = preview;
    }
    return _previewLayer;
}
    
- (AVCaptureVideoOrientation)currentVideoOrientation {
    
    AVCaptureVideoOrientation orientation;
    
    switch ([UIDevice currentDevice].orientation) {                         // 3
        case UIDeviceOrientationPortrait:
        orientation = AVCaptureVideoOrientationPortrait;
        break;
        case UIDeviceOrientationLandscapeRight:
        orientation = AVCaptureVideoOrientationLandscapeLeft;
        break;
        case UIDeviceOrientationPortraitUpsideDown:
        orientation = AVCaptureVideoOrientationPortraitUpsideDown;
        break;
        default:
        orientation = AVCaptureVideoOrientationLandscapeRight;
        break;
    }
    
    return orientation;
}

@end
