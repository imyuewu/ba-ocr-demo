#include "BaFeature.h"

void getHOGFeature(const IplImage *srcImage, CvMat *featureMat) {
    
}

void getXMapFeature(const IplImage *srcImage, CvMat *featureMat) {
    CvSize srcSize = cvGetSize(srcImage);
    double *matDataPtr = (double *)(featureMat->data.ptr + 0 * featureMat->step);
    for (int i = 0; i < srcSize.width; i++) {
        int whitePixCount = 0;
        for (int j = 0; j < srcSize.height; j++) {
            unsigned char *line = (unsigned char *)(srcImage->imageData + j * srcImage->widthStep);
            if (line[i] == 255) {
                whitePixCount++;
            }
        }
        matDataPtr[i] = (float)whitePixCount;
    }
}
