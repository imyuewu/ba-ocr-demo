#ifndef __BA_FILE_UTILS_H__
#define __BA_FILE_UTILS_H__

#include "BaOcrHeaders.h"

extern const char *g_srcImgPath;

bool isImageFile(const char *filePath);
bool isFileExists(const char *filePath);
bool isDirectory(const char *dirPath);
void getFileExt(const char *filePath, char *fileExt);

void saveImage(const char *filePath, IplImage *img);
void saveProcessImage(IplImage *img, const char *ext);
void genGrayHistImage(const char *outImgPath, const CvHistogram *hist, CvSize srcImgSize);
void genBinMap(const char *imgPath, const char *outImgPath);
void genGrayHistImageByFile(const char *imgPath, const char *outImgPath);

#endif
