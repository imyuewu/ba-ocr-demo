#ifndef __BA_FILE_UTILS_H__
#define __BA_FILE_UTILS_H__

#include "BaOcrHeaders.h"

extern const char *g_srcImgPath;

void appendCharsIntoFilePath(const char *srcStr, const char *insertStr, char *resultStr);
char *genFilePath(const char *filePath, const char *pathExt);

void saveImage(const char *filePath, IplImage *img);
void saveProcessImage(IplImage *img, const char *ext);
void genGrayHistImage(const char *outImgPath, const CvHistogram *hist, CvSize srcImgSize);
void genBinMap(const char *imgPath, const char *outImgPath);
void genGrayHistImageByFile(const char *imgPath, const char *outImgPath);

#endif
