//
//  OcrCaptureView.m
//  OcrDemo
//
//  Created by nero on 18/1/3.
//  Copyright © 2018年 Technology Co., Ltd. http://www.imwooy.com. All rights reserved.
//

#import "OcrCaptureView.h"

#define STD_SCALE 0.57
#define STD_HEIGHT 460
#define STD_DEVICE_RATIO (SCREEN_HEIGHT / 568)

@interface OcrCaptureView ()

@property (nonatomic, strong) OcrCaptureManager *captureManager;
@property (nonatomic, strong) UIButton *backBtn;
@property (nonatomic, strong) UIButton *captureBtn;

@end

@implementation OcrCaptureView
    
- (void)dealloc {
    NSLog(@"************ Ocr Capture View Dead ************** ");
}
    
- (void)initUI {
    // add rect
    UIView *targetRect = [[UIView alloc] init];
    [targetRect setBackgroundColor:[UIColor clearColor]];
    [targetRect.layer setBorderWidth:1.0f];
    [targetRect.layer setBorderColor:[[UIColor orangeColor] CGColor]];
    [self addSubview:targetRect];
    NSLog(@"%@", [NSValue valueWithCGRect:self.frame]);
    float height = STD_HEIGHT * STD_DEVICE_RATIO;
    float width = height * STD_SCALE;
    float widthRatio = width / self.frame.size.width;
    float heightRatio = height / self.frame.size.height;
    [self.captureManager setSizeRatio:CGSizeMake(widthRatio, heightRatio)];
    [targetRect mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(CGSizeMake(width, height));
        make.centerX.equalTo(self);
        make.bottom.equalTo(self).offset(-70);
    }];
    // add mask
    UIView *mask1 = [[UIView alloc] init];
    mask1.backgroundColor = [UIColor lightGrayColor];
    mask1.alpha = 0.9;
    [self addSubview:mask1];
    [mask1 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.top.right.equalTo(self);
        make.width.equalTo(self);
        make.bottom.equalTo(targetRect.mas_top);
    }];
    UIView *mask3 = [[UIView alloc] init];
    mask3.backgroundColor = mask1.backgroundColor;
    mask3.alpha = mask1.alpha;
    [self addSubview:mask3];
    [mask3 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.bottom.right.equalTo(self);
        make.width.equalTo(self);
        make.top.equalTo(targetRect.mas_bottom);
    }];
    UIView *mask2 = [[UIView alloc] init];
    mask2.backgroundColor = mask1.backgroundColor;
    mask2.alpha = mask1.alpha;
    [self addSubview:mask2];
    [mask2 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(mask1.mas_bottom);
        make.left.equalTo(self);
        make.right.equalTo(targetRect.mas_left);
        make.bottom.equalTo(mask3.mas_top);
    }];
    UIView *mask4 = [[UIView alloc] init];
    mask4.backgroundColor = mask1.backgroundColor;
    mask4.alpha = mask1.alpha;
    [self addSubview:mask4];
    [mask4 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.bottom.equalTo(mask2);
        make.left.equalTo(targetRect.mas_right);
        make.right.equalTo(self);
    }];

    [self addSubview:self.backBtn];
    [self.backBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.bottom.equalTo(self.mas_bottom).offset(-25);
        make.left.equalTo(self).offset(10);
        make.size.mas_equalTo(CGSizeMake(60, 20));
    }];
    
    [self addSubview:self.captureBtn];
    [self.captureBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.equalTo(self);
        make.bottom.equalTo(self).offset(-8);
        make.size.mas_equalTo(CGSizeMake(50, 50));
    }];
    [self.captureBtn setNeedsLayout];
    [self.captureBtn layoutIfNeeded];
    [self.captureBtn.layer setCornerRadius:25];
}
- (void)initCamera {
    self.captureManager = [[OcrCaptureManager alloc] init];
    [self.captureManager initCameraWithRect:CGRectMake(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)];
    [self.layer setNeedsLayout];
    [self.layer layoutIfNeeded];
    NSLog(@"x = %f, y = %f, width = %.2f, height = %.2f", self.layer.frame.origin.x, self.layer.frame.origin.y, self.layer.frame.size.width, self.layer.frame.size.height);
    [self.layer addSublayer:self.captureManager.previewLayer];
}

- (void)takePhoto:(CaptureBlock)block {
    [self.captureManager takePic:block];
}
    
- (void)didClickBackBtn {
    if ([self.delegate respondsToSelector:@selector(clickBackBtn:)]) {
        [self.delegate clickBackBtn:self];
    }
}
    
- (void)didClickCaptureBtn {
    if ([self.delegate respondsToSelector:@selector(clickCaptureBtn:)]) {
        [self.delegate clickCaptureBtn:self];
    }
}

- (UIButton *)captureBtn {
    if (!_captureBtn) {
        _captureBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        _captureBtn.alpha = 0.5;
        [_captureBtn setBackgroundColor:[UIColor blackColor]];
        [_captureBtn addTarget:self action:@selector(didClickCaptureBtn) forControlEvents:UIControlEventTouchUpInside];
    }
    return _captureBtn;
}
    
- (UIButton *)backBtn {
    if (!_backBtn) {
        _backBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        [_backBtn setTitle:@"返回" forState:UIControlStateNormal];
        [_backBtn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
        [_backBtn.titleLabel setFont:[UIFont systemFontOfSize:16.0f]];
        [_backBtn setBackgroundColor:[UIColor clearColor]];
        [_backBtn addTarget:self action:@selector(didClickBackBtn) forControlEvents:UIControlEventTouchUpInside];
    }
    return _backBtn;
}

@end
