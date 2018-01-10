//
//  FileUtils.m
//  OcrDemo
//
//  Created by nero on 18/1/5.
//  Copyright © 2018年 Technology Co., Ltd. http://www.imwooy.com. All rights reserved.
//

#import "FileUtils.h"

@implementation FileUtils

+ (NSString *)homePath
{
    return NSHomeDirectory();
}

+ (NSString *)appPath
{
    NSArray *pathArr = NSSearchPathForDirectoriesInDomains(NSApplicationDirectory, NSUserDomainMask, YES);
    return [pathArr objectAtIndex:0];
}

+ (NSString *)docPath
{
    NSArray *pathArr = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    return [pathArr objectAtIndex:0];
}

+ (NSString *)libPrefPath
{
    NSArray *pathArr = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
    return [[pathArr objectAtIndex:0] stringByAppendingPathComponent:@"/Preference"];
}

+ (NSString *)libCachePath
{
    NSArray *pathArr = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
    
    return [[pathArr objectAtIndex:0] stringByAppendingPathComponent:@"/Caches"];
}

+ (NSString *)cachePath {
    NSArray *pathArr = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    return [pathArr objectAtIndex:0];
}

+ (NSString *)tmpPath
{
    return [NSHomeDirectory() stringByAppendingPathComponent:@"/tmp"];
}

+ (BOOL)isFileExistsWithPath:(NSString *)filePath {
    NSFileManager *fileManager = [NSFileManager defaultManager];
    return [fileManager fileExistsAtPath:filePath];
}


+ (BOOL)isDirectoryExistsWithPath:(NSString *)path {
    NSFileManager *fileManager = [NSFileManager defaultManager];
    BOOL isDirectory = YES;
    return [fileManager fileExistsAtPath:path isDirectory:&isDirectory];
}

+ (void)deleteFileAtPath:(NSString *)path {
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if ([fileManager fileExistsAtPath:path]) {
        [fileManager removeItemAtPath:path error:nil];
    }
}

+ (NSString *)genImageSavePath {
    NSDateFormatter *df = [[NSDateFormatter alloc] init];
    [df setDateFormat:@"yyyyMMdd_HHmmssSSS"];
    NSString *fileName = [NSString stringWithFormat:@"image_%@.jpg", [df stringFromDate:[NSDate date]]];
    NSString *fullPath = [[FileUtils baImageDocPath] stringByAppendingPathComponent:[NSString stringWithFormat:@"%@", fileName]];
    return fullPath;
}

+ (NSString *)baImageDocPath {
    NSString *imageDocName = @"baimg";
    NSString *imageDocPath = [[FileUtils libCachePath] stringByAppendingPathComponent:[NSString stringWithFormat:@"%@", imageDocName]];
    NSFileManager *fileManager = [NSFileManager defaultManager];
    BOOL isDir = YES;
    if (![fileManager fileExistsAtPath:imageDocPath isDirectory:&isDir]) {
        [fileManager createDirectoryAtPath:imageDocPath withIntermediateDirectories:YES attributes:nil error:nil];
    }
    return imageDocPath;
}
    
+ (void)copyTrainedData:(NSString *)srcPath {
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSString *engDataPath = [[FileUtils docPath] stringByAppendingPathComponent:@"tessdata/eng.traineddata"];
    if (![fileManager fileExistsAtPath:engDataPath]) {
        NSString *trainedDataDocPath = [[FileUtils docPath] stringByAppendingPathComponent:@"tessdata"];
        BOOL isDir = true;
        if (![fileManager fileExistsAtPath:trainedDataDocPath isDirectory:&isDir]) {
            [fileManager createDirectoryAtPath:trainedDataDocPath withIntermediateDirectories:YES attributes:nil error:NULL];
        }
        NSData *data = [NSData dataWithContentsOfFile:srcPath];
        [data writeToFile:engDataPath atomically:NO];
//        [fileManager copyItemAtPath:srcPath toPath:engDataPath error:NULL];
    }
    NSLog(@"tessdata path : %@", engDataPath);
//    setenv("TESSDATA_PREFIX", [[documentPath stringByAppendingString:@"/"] UTF8String], 1);
}
    
+ (bool)isSerialNum:(NSString *)str {
    bool result = false;
    if (![str containsString:@" "]) result = true;
    return result;
}

@end
