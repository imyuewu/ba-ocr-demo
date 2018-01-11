#ifndef __BA_DETECT_H__
#define __BA_DETECT_H__

#include "BaImgOpr.h"

void searchTitleBySobel(const IplImage *srcImg, CvSeq *results);
void searchTitleByMorph(const IplImage *srcImg, CvSeq *results);
void searchSerialBySobel(const IplImage *srcImg, CvSeq *results);
void searchSerialByMorph(const IplImage *srcImg, CvSeq *results);

#endif
