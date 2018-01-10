#ifndef __BA_IMG_OPR_H__
#define __BA_IMG_OPR_H__


#include "BaOcrHeaders.h"
#include "BaAlgo.h"
#include "BaStrUtils.h"
#include "BaFileUtils.h"
#include "BaImgUtils.h"

typedef struct ContourRes {
    CvRect rect;
    float angle;
} ContourRes;

int preProcImg(IplImage **pSrcImg);
int morphOprTitleArea(const IplImage *srcImg, IplImage *dstImg);
int sobelOprTitleArea(const IplImage *srcImg, IplImage *dstImg);
int morphOprSerialArea(const IplImage *srcImg, IplImage *dstImg);
int sobelOprSerialArea(const IplImage *srcImg, IplImage *dstImg);

/*========== Image Utils ==========*/
void adjContrastAndBright(const IplImage *srcImg, IplImage *dstImg);
void adjContrastAndBright2(const IplImage *srcImg, IplImage *dstImg);
// 噪点过滤
void filterNosiyPoint(IplImage *srcImg);
void filterTitleContours(IplImage *srcImg, CvSeq *results);
void findoutSerialContours(const IplImage *srcImg, CvSeq *results);



#endif
