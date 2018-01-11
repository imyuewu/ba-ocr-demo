#include "BaImgUtils.h"

void showImageInfo(const IplImage *img) {
    printf("\n========== Image Info ==========\n");
    printf("size: %d x %d\n", img->width, img->height);
    printf("channels: %d, alpha channel: %d\n", img->nChannels, img->alphaChannel);
    printf("depth: %d, ", img->depth);
    if (img->depth == IPL_DEPTH_8U) {
        printf("8u\n");
    } else if (img->depth == IPL_DEPTH_8S) {
        printf("8s\n");
    } else if (img->depth == IPL_DEPTH_16S) {
        printf("16s\n");
    } else if (img->depth == IPL_DEPTH_32S) {
        printf("32s\n");
    } else if (img->depth == IPL_DEPTH_32F) {
        printf("32f\n");
    } else if (img->depth == IPL_DEPTH_64F) {
        printf("64f\n");
    } else {
        printf("Unknown\n");
    }
    printf("widthStep: %d\n", img->widthStep);
    // 理论上widthStep = depth / 8 * nChannels * width
    // 但是由于存在字节对齐问题，实际上widthStep是要比计算值要大的
    printf("depth * nChannels * width / 8 = %d\n", img->depth * img->nChannels * img->width / 8);
    printf("origin: ");
    if (img->origin == IPL_ORIGIN_TL) {
        printf("top left\n");
    } else if (img->origin == IPL_ORIGIN_BL) {
        printf("bottom left\n");
    } else {
        printf("Unknown\n");
    }
    printf("dataOrder: ");
    if (img->dataOrder == IPL_DATA_ORDER_PIXEL) {
        printf("pixel\n");
    } else if (img->dataOrder == IPL_DATA_ORDER_PLANE) {
        printf("plane\n");
    } else {
        printf("Unknown\n");
    }
    printf("================================\n\n");
}

void rotateImg(const IplImage *srcImg, IplImage *dstImg, int angle) {
    CvMat *rotationMat = cvCreateMat(2, 3, CV_32FC1);
    CvPoint2D32f center = cvPoint2D32f((float)srcImg->width / 2, (float)srcImg->height / 2);
    double scale = 1.0;
    cv2DRotationMatrix(center, angle, scale, rotationMat);
    cvWarpAffine(srcImg, dstImg, rotationMat, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(255));
    cvReleaseMat(&rotationMat);
}

void rotateImg90n(const IplImage *srcImg, IplImage *dstImg, int angle) {
    assert(angle % 90 == 0 && angle <= 360 && angle >= -360);
    if (angle == 270 || angle == -90) {
        cvTranspose(srcImg, dstImg);
        cvFlip(dstImg, dstImg, 0);
    } else if (angle == 180 || angle == -180) {
        cvFlip(srcImg, dstImg, -1);
    } else if (angle == 90 || angle == -270) {
        cvTranspose(srcImg, dstImg);
        cvFlip(dstImg, dstImg, 1);
    } else if (angle == 0 || angle == 360 || angle == -360) {
        if (srcImg->imageData != dstImg->imageData) {
            cvCopy(srcImg, dstImg, NULL);
        }
    }
}

void getRedChannelImage(const IplImage *srcImg, IplImage *dstImg) {
    IplImage *imgB = cvCreateImage(cvGetSize(srcImg), srcImg->depth, 1);
    IplImage *imgG = cvCreateImage(cvGetSize(srcImg), srcImg->depth, 1);
    cvSplit(srcImg, imgB, imgG, dstImg, NULL);
    cvReleaseImage(&imgB);
    cvReleaseImage(&imgG);
}

void meanShiftImageForSerial(const IplImage *srcImg, IplImage *dstImg) {
    CvSize scSrc = cvGetSize(srcImg);
    IplImage *pl3CImg1 = cvCreateImage(scSrc, srcImg->depth, 3);
    IplImage *pl3CImg2 = cvCreateImage(scSrc, srcImg->depth, 3);
    cvCvtColor(srcImg, pl3CImg1, CV_GRAY2BGR);
    cvPyrMeanShiftFiltering(pl3CImg1, pl3CImg2, 3, 35, 3, cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 5, 1));
    cvCvtColor(pl3CImg2, dstImg, CV_BGR2GRAY);
    cvReleaseImage(&pl3CImg1);
    cvReleaseImage(&pl3CImg2);
}

void adjContrastAndBrightForSerial(const IplImage *srcImg, IplImage *dstImg) {
    static const double kDefaultMean = 180;
    static const double kDefaultSdv = 45;
    CvSize szSrc = cvGetSize(srcImg);
    CvScalar scSampleMean;
    CvScalar scSampleSdv;
    CvRect rtSample = cvRect((szSrc.width - 150) / 2, szSrc.height - 150, 150, 150);
    IplImage *tmpImg = cvCreateImage(szSrc, srcImg->depth, srcImg->nChannels);
    cvCopy(srcImg, tmpImg, NULL);
    cvSetImageROI(tmpImg, rtSample);
    // scSampleMean = cvAvg(tmpImg, NULL);
    cvAvgSdv(tmpImg, &scSampleMean, &scSampleSdv, NULL);
    cvResetImageROI(tmpImg);
    cvReleaseImage(&tmpImg);
    fprintf(stdout, "sample rect gray mean: %.2f, sdv: %.2f\n", scSampleMean.val[0], scSampleSdv.val[0]);
    double contrastRatio = 1.3;
    double brightRatio = 1.0;
    double mean = scSampleMean.val[0];
    double sdv = scSampleSdv.val[0];
    brightRatio -= (mean - kDefaultMean) / kDefaultMean;
    // contrastRatio -= ((sdv - kDefaultSdv) >= 0 ? 0 : (sdv - kDefaultSdv)) / (kDefaultSdv / 3 * 2);
    contrastRatio -= (sdv - kDefaultSdv) / (kDefaultSdv / 3 * 2);

    for (int i = 0; i < srcImg->height; i++) {
        unsigned char *lineSrc = (unsigned char *)(srcImg->imageData + srcImg->widthStep * i);
        unsigned char *lineDst = (unsigned char *)(dstImg->imageData + dstImg->widthStep * i);
        for (int j = 0; j < srcImg->width; j++) {
            int tmp = (int)((lineSrc[j] - mean) * contrastRatio + mean * brightRatio);
            lineDst[j] = (unsigned char)(tmp > 255 ? 255 : (tmp < 0 ? 0 : tmp));
        }
    }
}

void adjContrastAndBrightForSerialChar(const IplImage *srcImg, IplImage *dstImg) {
    static const double kDefaultMean = 180;
    static const double kDefaultSdv = 45;
    CvScalar scSampleMean;
    CvScalar scSampleSdv;
    // scSampleMean = cvAvg(tmpImg, NULL);
    cvAvgSdv(srcImg, &scSampleMean, &scSampleSdv, NULL);
    fprintf(stdout, "sample rect gray mean: %.2f, sdv: %.2f\n", scSampleMean.val[0], scSampleSdv.val[0]);
    double contrastRatio = 1.3;
    double brightRatio = 1.0;
    double mean = scSampleMean.val[0];
    double sdv = scSampleSdv.val[0];
    brightRatio -= (mean - kDefaultMean) / kDefaultMean;
    // contrastRatio -= ((sdv - kDefaultSdv) >= 0 ? 0 : (sdv - kDefaultSdv)) / (kDefaultSdv / 3 * 2);
    contrastRatio -= (sdv - kDefaultSdv) / (kDefaultSdv / 3 * 2);

    for (int i = 0; i < srcImg->height; i++) {
        unsigned char *lineSrc = (unsigned char *)(srcImg->imageData + srcImg->widthStep * i);
        unsigned char *lineDst = (unsigned char *)(dstImg->imageData + dstImg->widthStep * i);
        for (int j = 0; j < srcImg->width; j++) {
            int tmp = (int)((lineSrc[j] - mean) * contrastRatio + mean * brightRatio);
            lineDst[j] = (unsigned char)(tmp > 255 ? 255 : (tmp < 0 ? 0 : tmp));
        }
    }
}

void filterNosiyPointForSerial(const IplImage *srcImg, IplImage *dstImg) {
    CvMemStorage *memStorage = cvCreateMemStorage(0);
    IplImage *cpSrcImg = cvCreateImage(cvGetSize(srcImg), srcImg->depth, srcImg->nChannels);
    cvCopy(srcImg, cpSrcImg, NULL);

    CvSeq *cont = NULL;
    CvContourScanner contScanner = NULL;
    contScanner = cvStartFindContours(cpSrcImg, memStorage, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
    while ((cont = cvFindNextContour(contScanner)) != NULL) {
        double area = cvContourArea(cont, CV_WHOLE_SEQ, 0);
        CvRect rect = cvBoundingRect(cont, 0);
        if (area < 40 || area > 2400) {
            for (int i = 0; i < rect.height; i++) {
                unsigned char *ptr = (unsigned char *)(dstImg->imageData + dstImg->widthStep * (rect.y + i));
                for (int j = 0; j < rect.width; j++) {
                    ptr[rect.x + j] = 0;
                }
            }
        } else {
            float scale = (float)rect.width / rect.height;
            if (scale > 10.0) {
                for (int i = 0; i < rect.height; i++) {
                    unsigned char *ptr = (unsigned char *)(dstImg->imageData + dstImg->widthStep * (rect.y + i));
                    for (int j = 0; j < rect.width; j++) {
                        ptr[rect.x + j] = 0;
                    }
                }
            }
        }
    }
    // release contour scanner
    cvEndFindContours(&contScanner);

    cvClearMemStorage(memStorage);
    cvReleaseMemStorage(&memStorage);
    cvReleaseImage(&cpSrcImg);
}
