//
//  OcrCaptureManager.m
//  OcrDemo
//
//  Created by nero on 18/1/3.
//  Copyright © 2018年 Technology Co., Ltd. http://www.imwooy.com. All rights reserved.
//

#import "OcrCaptureManager.h"

// 角度和弧度转换
#define DEGREES_TO_RADIANS(degrees)  ((M_PI * degrees) / 180.0)
#define RADIANS_TO_DEGREES(radians) ((radians * 180.0) / M_PI)

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
    
//    if (connection.isVideoOrientationSupported) {
//        connection.videoOrientation = [self currentVideoOrientation];
//    }
    
    id handler = ^(CMSampleBufferRef imageDataSampleBuffer, NSError *error) {
        if (imageDataSampleBuffer != NULL) {
            NSData *imageData = [AVCaptureStillImageOutput jpegStillImageNSDataRepresentation:imageDataSampleBuffer];
            UIImage *image = [[UIImage alloc] initWithData:imageData];
//            NSLog(@"originImage:%@", [NSValue valueWithCGSize:image.size]);
            UIImage *rotatedImage = [self rotateImageIfNessary:image];
//            NSLog(@"rotatedImage:%@, orientation: %ld", [NSValue valueWithCGSize:rotatedImage.size], rotatedImage.imageOrientation);
            UIImage *croppedImage = [self croppedImageIfNessary:rotatedImage];
            NSString *path = [FileUtils genImageSavePath];
            [UIImageJPEGRepresentation(croppedImage, 1.0f) writeToFile:path atomically:NO];
            if (captureBlock) {
                captureBlock(path);
            }
        }
    };
    
    // Capture still image
    [self.output captureStillImageAsynchronouslyFromConnection:connection completionHandler:handler];

}

- (UIImage *)rotateImageIfNessary:(UIImage *)srcImage {
    NSLog(@"orientation : %ld", (long)srcImage.imageOrientation);
    CGSize srcSize = srcImage.size;
    if (srcSize.width < srcSize.height && srcImage.imageOrientation == UIImageOrientationRight) {
        UIImage *dstImage = [UIImage imageWithCGImage:srcImage.CGImage scale:1 orientation:UIImageOrientationUp];
        UIImage *fixRotateImage = [self fixOrientationOfImage:dstImage];

        return fixRotateImage;
    }
    return srcImage;
}

- (UIImage *)croppedImageIfNessary:(UIImage *)srcImage {
    CGSize srcSize = srcImage.size;
    CGSize dstSize = CGSizeMake(srcSize.width * self.sizeRatio.width, srcSize.height * self.sizeRatio.height);
    CGRect dstRect = CGRectMake((srcSize.width - dstSize.width) / 2.0, (srcSize.height - dstSize.height) / 2.0, dstSize.width, dstSize.height);
    
    return [srcImage croppedImage:dstRect];
}

- (UIImage *)fixOrientationOfImage:(UIImage *)image {
    
    // No-op if the orientation is already correct
    if (image.imageOrientation == UIImageOrientationUp) return image;
    
    // We need to calculate the proper transformation to make the image upright.
    // We do it in 2 steps: Rotate if Left/Right/Down, and then flip if Mirrored.
    CGAffineTransform transform = CGAffineTransformIdentity;
    
    switch (image.imageOrientation) {
        case UIImageOrientationDown:
        case UIImageOrientationDownMirrored:
            transform = CGAffineTransformTranslate(transform, image.size.width, image.size.height);
            transform = CGAffineTransformRotate(transform, M_PI);
            break;
            
        case UIImageOrientationLeft:
        case UIImageOrientationLeftMirrored:
            transform = CGAffineTransformTranslate(transform, image.size.width, 0);
            transform = CGAffineTransformRotate(transform, M_PI_2);
            break;
            
        case UIImageOrientationRight:
        case UIImageOrientationRightMirrored:
            transform = CGAffineTransformTranslate(transform, 0, image.size.height);
            transform = CGAffineTransformRotate(transform, -M_PI_2);
            break;
        case UIImageOrientationUp:
        case UIImageOrientationUpMirrored:
            break;
    }
    
    switch (image.imageOrientation) {
        case UIImageOrientationUpMirrored:
        case UIImageOrientationDownMirrored:
            transform = CGAffineTransformTranslate(transform, image.size.width, 0);
            transform = CGAffineTransformScale(transform, -1, 1);
            break;
            
        case UIImageOrientationLeftMirrored:
        case UIImageOrientationRightMirrored:
            transform = CGAffineTransformTranslate(transform, image.size.height, 0);
            transform = CGAffineTransformScale(transform, -1, 1);
            break;
        case UIImageOrientationUp:
        case UIImageOrientationDown:
        case UIImageOrientationLeft:
        case UIImageOrientationRight:
            break;
    }
    
    // Now we draw the underlying CGImage into a new context, applying the transform
    // calculated above.
    CGContextRef ctx = CGBitmapContextCreate(NULL, image.size.width, image.size.height,
                                             CGImageGetBitsPerComponent(image.CGImage), 0,
                                             CGImageGetColorSpace(image.CGImage),
                                             CGImageGetBitmapInfo(image.CGImage));
    CGContextConcatCTM(ctx, transform);
    switch (image.imageOrientation) {
        case UIImageOrientationLeft:
        case UIImageOrientationLeftMirrored:
        case UIImageOrientationRight:
        case UIImageOrientationRightMirrored:
            // Grr...
            CGContextDrawImage(ctx, CGRectMake(0,0,image.size.height,image.size.width), image.CGImage);
            break;
            
        default:
            CGContextDrawImage(ctx, CGRectMake(0,0,image.size.width,image.size.height), image.CGImage);
            break;
    }
    
    // And now we just create a new UIImage from the drawing context
    CGImageRef cgimg = CGBitmapContextCreateImage(ctx);
    UIImage *img = [UIImage imageWithCGImage:cgimg];
    CGContextRelease(ctx);
    CGImageRelease(cgimg);
    return img;
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
