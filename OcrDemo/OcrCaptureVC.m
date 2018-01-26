//
//  OcrCaptureVC.m
//  OcrDemo
//
//  Created by nero on 18/1/3.
//  Copyright © 2018年 Technology Co., Ltd. http://www.imwooy.com. All rights reserved.
//

#import "OcrCaptureVC.h"

@interface OcrCaptureVC ()

@end

@implementation OcrCaptureVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    [self.view addSubview:self.ocrCaptureView];
    [self.ocrCaptureView setDelegate:self];
    [self.ocrCaptureView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.equalTo(self.view);
        make.edges.equalTo(self.view);
    }];
    
    [self.ocrCaptureView initCamera];
    [self.ocrCaptureView initUI];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)clickBackBtn:(OcrCaptureView *)captureView {
    [self dismissViewControllerAnimated:YES completion:nil];
}
- (void)clickCaptureBtn:(OcrCaptureView *)captureView {
    [self.ocrCaptureView takePhoto:^(NSString *imagePath) {
//        [NSBundle allFrameworks];
        BaOcrResOC *result = [BaOcrApiOC getBASerialOC:imagePath];
        NSString *mess = nil;
        if (result.errCode == NO_ERROR) {
            mess = result.serial;
        } else {
            mess = @"检测失败！";
        }

        UIAlertController *alerCtrl = [UIAlertController alertControllerWithTitle:@"结果" message:mess preferredStyle:UIAlertControllerStyleAlert];
        UIAlertAction *action = [UIAlertAction actionWithTitle:@"好的" style:UIAlertActionStyleCancel handler:nil];
        [alerCtrl addAction:action];
        [self presentViewController:alerCtrl animated:YES completion:nil];
    }];
}

- (OcrCaptureView *)ocrCaptureView {
    if (!_ocrCaptureView) {
        _ocrCaptureView = [[OcrCaptureView alloc] init];
    }
    return _ocrCaptureView;
}


@end
