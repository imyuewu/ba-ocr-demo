//
//  OcrCaptureView.h
//  OcrDemo
//
//  Created by nero on 18/1/3.
//  Copyright © 2018年 Technology Co., Ltd. http://www.imwooy.com. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "OcrCaptureManager.h"

@class OcrCaptureView;

@protocol OcrCaptureViewDelegate <NSObject>

@required
- (void)clickBackBtn:(OcrCaptureView *)captureView;
- (void)clickCaptureBtn:(OcrCaptureView *)captureView;

@end

@interface OcrCaptureView : UIView

@property (nonatomic, weak) id<OcrCaptureViewDelegate> delegate;
    
- (void)initCamera;
- (void)initUI;
- (void)takePhoto:(CaptureBlock)block;
@end
