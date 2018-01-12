//
//  RootVC.m
//  OcrDemo
//
//  Created by nero on 18/1/3.
//  Copyright © 2018年 Technology Co., Ltd. http://www.imwooy.com. All rights reserved.
//

#import "RootVC.h"
#import "OcrCaptureVC.h"

@interface RootVC ()

@end

@implementation RootVC

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    self.view.backgroundColor = [UIColor whiteColor];
    
    UIButton *btn = [[UIButton alloc] init];
    [self.view addSubview:btn];
    [btn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.mas_equalTo(CGSizeMake(160, 40));
        make.centerX.equalTo(self.view);
        make.top.equalTo(self.view).offset(50.0f);
    }];
    [btn addTarget:self action:@selector(didClickTakePhotoBtn) forControlEvents:UIControlEventTouchUpInside];
    [btn setTitle:@"拍摄票据" forState:UIControlStateNormal];
    [btn setTitleColor:[UIColor orangeColor] forState:UIControlStateNormal];
    btn.layer.borderWidth = 2.0f;
    btn.layer.borderColor = [[UIColor orangeColor] CGColor];
    
    UIButton *btn2 = [[UIButton alloc] init];
    [self.view addSubview:btn2];
    [btn2 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.size.equalTo(btn);
        make.centerX.equalTo(self.view);
        make.top.equalTo(btn.mas_bottom).offset(30.0f);
    }];
    [btn2 addTarget:self action:@selector(didClickTrainBtn) forControlEvents:UIControlEventTouchUpInside];
    [btn2 setTitle:@"开始训练" forState:UIControlStateNormal];
    [btn2 setTitleColor:[UIColor orangeColor] forState:UIControlStateNormal];
    btn2.layer.borderWidth = 2.0f;
    btn2.layer.borderColor = [[UIColor orangeColor] CGColor];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)didClickTakePhotoBtn {
    OcrCaptureVC *captureVC = [[OcrCaptureVC alloc] init];
    [self presentViewController:captureVC animated:YES completion:nil];
}

- (void)didClickTrainBtn {
    [BaOcrApiOC testTrain];
}

@end
