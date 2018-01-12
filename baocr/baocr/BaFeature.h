#ifndef __BA_FEATURE_H__
#define __BA_FEATURE_H__
#include "BaOcrHeaders.h"

#ifdef __cplusplus
extern "C" {
#endif

void getHOGFeature(const IplImage *srcImage, CvMat *featureMat);

/**
 * featureMat是一个 1 * srcImageWidth 的32FC1的Mat
 */
void getXMapFeature(const IplImage *srcImage, CvMat *featureMat);

#ifdef __cplusplus
}
#endif

#endif
