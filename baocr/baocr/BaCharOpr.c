#include "BaCharOpr.h"
#include "BaImgUtils.h"

void oprSerialImage(const IplImage *srcImg, IplImage *dstImg) {
    IplImage *plTempImage = cvCreateImage(cvGetSize(srcImg), srcImg->depth, srcImg->nChannels);
    // adj contrast
    adjContrastAndBrightForSerialChar(srcImg, plTempImage);
    // smooth
    cvSmooth(plTempImage, plTempImage, CV_GAUSSIAN, 3, 3, 0, 0);
    // binary
    cvThreshold(plTempImage, dstImg, 0, 255, CV_THRESH_OTSU | CV_THRESH_BINARY_INV);

    cvReleaseImage(&plTempImage);
}
