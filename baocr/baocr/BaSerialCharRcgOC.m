//
//  BaSerialCharRcgOC.m
//  baocr
//
//  Created by nero on 2018/1/9.
//  Copyright © 2018年 Yinchengku Technology Co., Ltd. http://www.yinchengku.com. All rights reserved.
//

#import "BaSerialCharRcgOC.h"
#import <TesseractOCR/TesseractOCR.h>

#define BAOCRFrameworkPath [[NSBundle bundleForClass:[self class]] pathForResource:@"baocr" ofType:@"framework"]
#define BAOCRBundlePath [[NSBundle bundleWithPath:BAOCRFrameworkPath] pathForResource:@"baocr" ofType:@"bundle"]

static BaSerialCharRcgOC *serialCharRcg = nil;

@interface BaSerialCharRcgOC ()

@property (nonatomic, strong) G8Tesseract *tesseract;
@property (nonatomic, strong) NSArray *cfgFileNames;
@property (nonatomic, strong) NSString *traineddataPath;

@end

@implementation BaSerialCharRcgOC

- (instancetype)init {
    return [BaSerialCharRcgOC sharedInstance];
}

- (instancetype)initSingleton {
    self = [super init];
    if (self) {
        _tesseract = [[G8Tesseract alloc] initWithLanguage:@"eng" configDictionary:nil configFileNames:self.cfgFileNames absoluteDataPath:nil engineMode:G8OCREngineModeTesseractOnly];
    }
    return self;
}

+ (BaSerialCharRcgOC *)sharedInstance {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        serialCharRcg = [[BaSerialCharRcgOC alloc] initSingleton];
    });
    return serialCharRcg;
}

- (NSString *)rcgEntiretySerial:(NSString *)imagePath {
    return [self rcgChar:imagePath];
}

- (NSString *)rcgSingleSerialChar:(NSString *)imagePath {
    return [self rcgChar:imagePath];
}

- (NSString *)rcgChar:(NSString *)imagePath {
    UIImage *image = [[UIImage alloc] initWithContentsOfFile:imagePath];
    self.tesseract.image = image;
    [self.tesseract recognize];
    
    return self.tesseract.recognizedText;
}

- (NSArray *)cfgFileNames {
    if (!_cfgFileNames) {
        NSString *tesscfgDocPath = [BAOCRBundlePath stringByAppendingPathComponent:@"tessconfigs"];
        NSString *serialcfgPath = [tesscfgDocPath stringByAppendingPathComponent:@"ba_serials"];
        NSFileManager *defaultFileManager = [NSFileManager defaultManager];
        if (![defaultFileManager fileExistsAtPath:serialcfgPath]) {
            NSLog(@"load tess configs fail!");
        }
        _cfgFileNames = @[serialcfgPath];
    }
    return _cfgFileNames;
}

- (NSString *)traineddataPath {
    if (!_traineddataPath) {
        NSString *tessdataDocPath = [BAOCRBundlePath stringByAppendingPathComponent:@"tessdata"];
        _traineddataPath = [tessdataDocPath stringByAppendingPathComponent:@"eng.traineddata"];
        NSFileManager *defaultFileManager = [NSFileManager defaultManager];
        if (![defaultFileManager fileExistsAtPath:_traineddataPath]) {
            NSLog(@"load traineddata fail!");
        }
    }
    return _traineddataPath;
}

@end
