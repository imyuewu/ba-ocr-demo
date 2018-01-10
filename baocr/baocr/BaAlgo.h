#ifndef __BA_ALGO_H__
#define __BA_ALGO_H__

#include "BaOcrHeaders.h"

void initGrayHist(CvHistogram **hist);
// Otsu 最大间方差
int calOtsuThreshlod(CvHistogram *hist);
// 当前像素最大熵
float calCurrentEntropy(CvHistogram *hist, int threshold);
// 最大熵
float calMaximumEntropy(CvHistogram *hist);
// 最大熵阈值
int calMaximumEntropyThreshold(CvHistogram *hist);
// 计算给定hist的信息熵
float calLineEntropy(int hist[], int size);
// 均值迭代阈值 initThreshold 一般为原图平均灰度值
int calMeanIterationThreshold(IplImage *srcImg, int initThreshold);

#endif
