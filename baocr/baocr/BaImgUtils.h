#ifndef __BA_IMG_UTILS_H__
#define __BA_IMG_UTILS_H__

#include "BaOcrHeaders.h"

void meanShiftImageForSerial(const IplImage *srcImg, IplImage *dstImg);
void adjContrastAndBrightForSerial(const IplImage *srcImg, IplImage *dstImg);
void adjContrastAndBrightForSerialChar(const IplImage *srcImg, IplImage *dstImg);
void filterNosiyPointForSerial(const IplImage *srcImg, IplImage *dstImg);

void showImageInfo(const IplImage *img);
void rotateImg90n(const IplImage *srcImg, IplImage *dstImg, int angle);
void rotateImg(const IplImage *srcImg, IplImage *dstImg, int angle);
void getRedChannelImage(const IplImage *srcImg, IplImage *dstImg);

#endif
