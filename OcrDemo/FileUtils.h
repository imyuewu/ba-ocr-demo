//
//  FileUtils.h
//  OcrDemo
//
//  Created by nero on 18/1/5.
//  Copyright © 2018年 Technology Co., Ltd. http://www.imwooy.com. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface FileUtils : NSObject

/** 程序主目录 下面有三个文件夹 Documents Library tmp 里面不可新建任何文件夹和文件
 *
 */
+ (NSString *)homePath;

/** 应用主目录 等同于上面路径
 *
 */
+ (NSString *)appPath;


/** Documents 文件夹 iTunes会备份 用户信息文件存放文件夹
 *
 */
+ (NSString *)docPath;

/** Library/Preferences文件夹 存放程序设置路径 里面放有appname.plist文件 iTunes会备份
 *
 */
+ (NSString *)libPrefPath;

/** Library/caches文件夹 程序缓存文件夹 程序退出不会删除 不会备份
 *
 */
+ (NSString *)libCachePath;


/**
 *  返回path和libCachePath相同(Library/caches)
 *
 */
+ (NSString *)cachePath;

/** 临时文件文件夹 手机重启删除 不会备份
 *
 */
+ (NSString *)tmpPath;

/**
 *  判断文件是否存在
 *
 *  @param filePath 文件路径
 *  @return YES 存在 NO 不存在
 */
+ (BOOL)isFileExistsWithPath:(NSString *)filePath;

/**
 *  判断目录是否存在
 *
 *  @param path 路径
 *  @return YES 存在 NO 不存在
 */
+ (BOOL)isDirectoryExistsWithPath:(NSString *)path;


/**
 *  删除文件
 *
 @param path 需要删除文件的path
 */
+ (void)deleteFileAtPath:(NSString *)path;

+ (NSString *)genImageSavePath;

+ (NSString *)baImageDocPath;
    
+ (void)copyTrainedData:(NSString *)srcPath;
    
+ (bool)isSerialNum:(NSString *)str;

@end
