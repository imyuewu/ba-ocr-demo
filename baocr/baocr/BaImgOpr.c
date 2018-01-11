#include "BaImgOpr.h"
#include "BaAlgo.h"
#include "BaStrUtils.h"
#include "BaFileUtils.h"
#include "BaImgUtils.h"
#include "BaType.h"

#ifdef DEBUG
// #define SAVE_PREPROCESS_IMAGE
#endif
int preProcImg(IplImage **pSrcImg) {
    int result = NO_ERROR;
    CvSize srcSize = cvGetSize(*pSrcImg);

#ifdef DEBUG
    float aspectRatio = (float)srcSize.width / (float)srcSize.height;
    fprintf(stdout, "image aspect ratio: %.2f\n", aspectRatio);
    fprintf(stdout, "src image size: [%d x %d]\n", srcSize.width, srcSize.height);
#endif
    //rotate image
    if (srcSize.height > srcSize.width) {
        IplImage *tmpImg = cvCreateImage(cvSize(srcSize.height, srcSize.width), (*pSrcImg)->depth, (*pSrcImg)->nChannels);
        rotateImg90n(*pSrcImg, tmpImg, -90);
        cvReleaseImage(pSrcImg);
        *pSrcImg = tmpImg;
        tmpImg = NULL;
        srcSize = cvGetSize(*pSrcImg);
    }

    // resize image
    CvSize dstSize = cvSize(0, 0);
    if (srcSize.width >= STD_WIDTH) {
        dstSize.width = STD_WIDTH;
    } else {
        result = IMAGE_SIZE_TOO_SMALL;
        return result;
    }

    float scale = (float)dstSize.width / srcSize.width;
    dstSize.height = scale * srcSize.height;

    IplImage *dstImg = cvCreateImage(dstSize, (*pSrcImg)->depth, (*pSrcImg)->nChannels);
    cvResize(*pSrcImg, dstImg, CV_INTER_AREA);
    cvReleaseImage(pSrcImg);
    *pSrcImg = dstImg;
    dstImg = NULL;

#ifdef DEBUG
        fprintf(stdout, "dst image size: [%d x %d]\n", dstSize.width, dstSize.height);
#endif
#ifdef SAVE_PREPROCESS_IMAGE
        saveProcessImage(*pSrcImg, "_resized");
#endif

    return result;
}

#ifdef DEBUG
#define SAVE_MORPH_TITLE_AREA_IMAGE
#endif
int morphOprTitleArea(const IplImage *srcImg, IplImage *dstImg) {
    int result = NO_ERROR;
    CvSize scSrc = cvGetSize(srcImg);
    IplImage *tmpImg = cvCreateImage(scSrc, srcImg->depth, srcImg->nChannels);
    cvCopy(srcImg, tmpImg, NULL);

    CvScalar mean;
    CvScalar sdv;
    cvAvgSdv(srcImg, &mean, &sdv, NULL);
#ifdef SAVE_MORPH_TITLE_AREA_IMAGE
    saveProcessImage(tmpImg, "_src");
#endif
#ifdef DEBUG
    fprintf(stdout, "src gray mean = %.2lf, std dev = %.2lf\n", mean.val[0], sdv.val[0]);
#endif
    // adjContrastAndBright(srcImg, tmpImg);
    adjContrastAndBright2(srcImg, tmpImg);
#ifdef SAVE_MORPH_TITLE_AREA_IMAGE
    saveProcessImage(tmpImg, "_adj_contrast");
#endif

    IplImage *pl3CImg1 = cvCreateImage(scSrc, tmpImg->depth, 3);
    IplImage *pl3CImg2 = cvCreateImage(scSrc, tmpImg->depth, 3);
    cvCvtColor(tmpImg, pl3CImg1, CV_GRAY2BGR);
    cvPyrMeanShiftFiltering(pl3CImg1, pl3CImg2, 5, 30, 3, cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS,5,1));
    cvCvtColor(pl3CImg2, tmpImg, CV_BGR2GRAY);
    saveProcessImage(tmpImg, "_mean");
    cvReleaseImage(&pl3CImg1);
    cvReleaseImage(&pl3CImg2);

    cvSmooth(tmpImg, tmpImg, CV_GAUSSIAN, 5, 5, 0, 0);
#ifdef SAVE_MORPH_TITLE_AREA_IMAGE
    saveProcessImage(tmpImg, "_smooth");
#endif

    CvPoint ptUp = cvPoint(scSrc.width / 2, 3);
    CvPoint ptDown = cvPoint(scSrc.width / 2, scSrc.height - 3);
    cvFloodFill(tmpImg, ptUp, cvScalarAll(255), cvScalarAll(2), cvScalarAll(2), NULL, 8, NULL);
    cvFloodFill(tmpImg, ptDown, cvScalarAll(255), cvScalarAll(2), cvScalarAll(2), NULL, 8, NULL);
#ifdef SAVE_MORPH_TITLE_AREA_IMAGE
    saveProcessImage(tmpImg, "_floodfill");
#endif
//     IplConvKernel *kenClose1 = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_RECT, NULL);
//     // cvDilate(tmpImg, tmpImg, kenClose1, 1);
//     cvMorphologyEx(tmpImg, tmpImg, NULL, kenClose1, CV_MOP_CLOSE, 1);
//     cvReleaseStructuringElement(&kenClose1);
// #ifdef SAVE_MORPH_TITLE_AREA_IMAGE
//     saveProcessImage(tmpImg, "_close1");
// #endif
    // CvSize szSrc = cvGetSize(tmpImg);
    // CvRect rtROI = cvRect(0, szSrc.height / 2 - 150, srcImg->width, 300);
    // IplImage *smpImg = cvCreateImage(cvSize(srcImg->width, 300), tmpImg->depth, 1);
    // cvSetImageROI(tmpImg, rtROI);
    // cvCopy(tmpImg, smpImg, NULL);
    // cvResetImageROI(tmpImg);

//
//     IplConvKernel *kenOpen1 = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_RECT, NULL);
//     cvMorphologyEx(tmpImg, tmpImg, NULL, kenOpen1, CV_MOP_OPEN, 1);
//     cvReleaseStructuringElement(&kenOpen1);
// #ifdef SAVE_MORPH_TITLE_AREA_IMAGE
//     saveProcessImage(tmpImg, "_open1");
// #endif

    // flood fill
//     CvPoint ptUp = cvPoint(tmpImg->width / 2, 5);
//     CvPoint ptDown = cvPoint(tmpImg->width / 5, tmpImg->height - 5);
//     cvFloodFill(tmpImg, ptUp, cvScalarAll(255), cvScalarAll(3), cvScalarAll(3), NULL, 8, NULL);
//     cvFloodFill(tmpImg, ptDown, cvScalarAll(255), cvScalarAll(3), cvScalarAll(3), NULL, 8, NULL);
// #ifdef SAVE_MORPH_TITLE_AREA_IMAGE
//     saveProcessImage(tmpImg, "_floodfill");
// #endif
    // calculate histogram
    CvHistogram *hist = NULL;
    initGrayHist(&hist);
    IplImage *images[] = {tmpImg};
    cvCalcHist(images, hist, 0, NULL);

#ifdef SAVE_MORPH_TITLE_AREA_IMAGE
    char *grayHistPath = genFilePath(g_srcImgPath, "_gray_hist");
    genGrayHistImage(grayHistPath, hist, cvGetSize(tmpImg));
    free(grayHistPath);
#endif

    // calculate threshold
    int threshold = calMaximumEntropyThreshold(hist);
    cvReleaseHist(&hist);
    // cvReleaseImage(&tmpImg);
#ifdef DEBUG
    CvScalar avgScalar = cvAvg(tmpImg, NULL);
    fprintf(stdout, "avg gray = %.0lf\n", avgScalar.val[0]);
    // int threshold = calMeanIterationThreshold(srcImgB, avgScalar.val[0]);
    fprintf(stdout, "threshold = %d\n", threshold);
#endif
    // to binary
    cvThreshold(tmpImg, tmpImg, threshold, 255, CV_THRESH_BINARY_INV);
    // cvThreshold(tmpImg, tmpImg, 0, 255, CV_THRESH_OTSU | CV_THRESH_BINARY_INV);
    filterNosiyPoint(tmpImg);
#ifdef SAVE_MORPH_TITLE_AREA_IMAGE
    saveProcessImage(tmpImg, "_binary");
#endif
// saveProcessImage(tmpImg, "_binary");

    IplConvKernel *knDilate = cvCreateStructuringElementEx(17, 3, 8, 1, CV_SHAPE_RECT, NULL);
    IplConvKernel *knErode = cvCreateStructuringElementEx(21, 7, 10, 3, CV_SHAPE_RECT, NULL);
    // IplConvKernel *knErode2 = cvCreateStructuringElementEx(5, 3, 0, 0, CV_SHAPE_RECT, NULL);
    cvDilate(tmpImg, tmpImg, knDilate, 1);
#ifdef SAVE_MORPH_TITLE_AREA_IMAGE
    saveProcessImage(tmpImg, "_dilate1");
#endif

    cvErode(tmpImg, tmpImg, knErode, 1);
#ifdef SAVE_MORPH_TITLE_AREA_IMAGE
    saveProcessImage(tmpImg, "_erode1");
#endif

    cvDilate(tmpImg, tmpImg, knDilate, 5);
#ifdef SAVE_MORPH_TITLE_AREA_IMAGE
    saveProcessImage(tmpImg, "_dilate2");
#endif
saveProcessImage(tmpImg, "_dilate2");

    cvCopy(tmpImg, dstImg, NULL);

    cvReleaseImage(&tmpImg);
    return result;
}

int sobelOprTitleArea(const IplImage *srcImg, IplImage *dstImg) {
    int result = NO_ERROR;
    return result;
}

#if defined DEBUG
#define SAVE_MORPH_SERIAL_AREA_IMAGE
#endif
int morphOprSerialArea(const IplImage *srcImg, IplImage *dstImg) {
    int result = NO_ERROR;
    CvSize scSrc = cvGetSize(srcImg);
    IplImage *tmpImg = cvCreateImage(scSrc, srcImg->depth, srcImg->nChannels);
    cvCopy(srcImg, tmpImg, NULL);

    adjContrastAndBrightForSerial(srcImg, tmpImg);
#ifdef SAVE_MORPH_SERIAL_AREA_IMAGE
    saveProcessImage(tmpImg, "_adj_contrast");
#endif
    meanShiftImageForSerial(tmpImg, dstImg);
#ifdef SAVE_MORPH_SERIAL_AREA_IMAGE
    saveProcessImage(dstImg, "_mean");
#endif

    CvPoint ptUp1 = cvPoint(3, 3);
    CvPoint ptUp2 = cvPoint(scSrc.width - 3, 3);
    CvPoint ptDown1 = cvPoint(3, scSrc.height - 3);
    CvPoint ptDown2 = cvPoint(scSrc.width - 3, scSrc.height - 3);
    cvFloodFill(dstImg, ptUp1, cvScalarAll(255), cvScalarAll(5), cvScalarAll(5), NULL, 8, NULL);
    cvFloodFill(dstImg, ptUp2, cvScalarAll(255), cvScalarAll(5), cvScalarAll(5), NULL, 8, NULL);
    cvFloodFill(dstImg, ptDown1, cvScalarAll(255), cvScalarAll(5), cvScalarAll(5), NULL, 8, NULL);
    cvFloodFill(dstImg, ptDown2, cvScalarAll(255), cvScalarAll(5), cvScalarAll(5), NULL, 8, NULL);
#ifdef SAVE_MORPH_SERIAL_AREA_IMAGE
    saveProcessImage(dstImg, "_floodfill");
#endif
    cvSmooth(dstImg, tmpImg, CV_GAUSSIAN, 3, 3, 0, 0);
// #ifdef SAVE_MORPH_SERIAL_AREA_IMAGE
//     saveProcessImage(tmpImg, "_smooth");
// #endif

    // calculate histogram
    CvHistogram *hist = NULL;
    initGrayHist(&hist);
    IplImage *images[] = {tmpImg};
    cvCalcHist(images, hist, 0, NULL);

// #ifdef SAVE_MORPH_SERIAL_AREA_IMAGE
//     char *grayHistPath = genFilePath(g_srcImgPath, "_gray_hist");
//     genGrayHistImage(grayHistPath, hist, cvGetSize(tmpImg));
//     free(grayHistPath);
// #endif

    // calculate threshold
    int threshold = calMaximumEntropyThreshold(hist);
    cvReleaseHist(&hist);
    // cvReleaseImage(&tmpImg);
#ifdef DEBUG
    CvScalar avgScalar = cvAvg(tmpImg, NULL);
    fprintf(stdout, "avg gray = %.0lf\n", avgScalar.val[0]);
    // int threshold = calMeanIterationThreshold(srcImgB, avgScalar.val[0]);
    fprintf(stdout, "threshold = %d\n", threshold);
#endif
    // to binary
    cvThreshold(tmpImg, tmpImg, threshold, 255, CV_THRESH_BINARY_INV);
    // cvThreshold(tmpImg, tmpImg, 0, 255, CV_THRESH_OTSU | CV_THRESH_BINARY_INV);
#ifdef SAVE_MORPH_SERIAL_AREA_IMAGE
    saveProcessImage(tmpImg, "_binary");
#endif
    filterNosiyPointForSerial(tmpImg, tmpImg);
#ifdef SAVE_MORPH_SERIAL_AREA_IMAGE
    saveProcessImage(tmpImg, "_nosiy");
#endif

    IplConvKernel *knDilate = cvCreateStructuringElementEx(13, 1, 13 / 2, 1 / 2, CV_SHAPE_RECT, NULL);
    IplConvKernel *knErode = cvCreateStructuringElementEx(15, 1, 15 / 2, 1 / 2, CV_SHAPE_RECT, NULL);
    IplConvKernel *knDilate2 = cvCreateStructuringElementEx(11, 1, 11 / 2, 1 / 2, CV_SHAPE_RECT, NULL);
    cvDilate(tmpImg, tmpImg, knDilate, 1);
#ifdef SAVE_MORPH_SERIAL_AREA_IMAGE
    saveProcessImage(tmpImg, "_dilate1");
#endif

    cvErode(tmpImg, tmpImg, knErode, 1);
#ifdef SAVE_MORPH_SERIAL_AREA_IMAGE
    saveProcessImage(tmpImg, "_erode1");
#endif

    cvDilate(tmpImg, tmpImg, knDilate2, 4);
#ifdef SAVE_MORPH_SERIAL_AREA_IMAGE
    saveProcessImage(tmpImg, "_dilate2");
#endif
    cvReleaseStructuringElement(&knDilate);
    cvReleaseStructuringElement(&knErode);
    cvReleaseStructuringElement(&knDilate2);
    cvCopy(tmpImg, dstImg, NULL);
    cvReleaseImage(&tmpImg);

    return result;
}

int sobelOprSerialArea(const IplImage *srcImg, IplImage *dstImg) {
    int result = NO_ERROR;
    return result;
}

/*========== Image Utils ==========*/

void adjContrastAndBright(const IplImage *srcImg, IplImage *dstImg) {
    static const double defaultMean = 180;
    // cal sample area gray avg
    CvSize szSrc = cvGetSize(srcImg);
    CvScalar scSampleMean;
    CvRect rtSample = cvRect(szSrc.width / 2 - 100, szSrc.height / 2 - 150, 200, 300);
    IplImage *tmpImg = cvCreateImage(szSrc, srcImg->depth, srcImg->nChannels);
    cvCopy(srcImg, tmpImg, NULL);
    cvSetImageROI(tmpImg, rtSample);
    scSampleMean = cvAvg(tmpImg, NULL);
    cvResetImageROI(tmpImg);
    cvReleaseImage(&tmpImg);
    fprintf(stdout, "sample rect gray mean: %.2f\n", scSampleMean.val[0]);

    int scMean = scSampleMean.val[0];
    double contrastRatio = 1.4;
    double brightRatio = 1.0;
    double cn, bn;
    cn = bn = 1;
    if (scMean - defaultMean > 0) {
        cn += (scMean - defaultMean) / 80;
    } else if (defaultMean - scMean > 0) {
        bn += (defaultMean - scMean) / 120;
    }
    // int mean = (int)scMean.val[0];
    for (int i = 0; i < srcImg->height; i++) {
        unsigned char *lineSrc = (unsigned char *)(srcImg->imageData + srcImg->widthStep * i);
        unsigned char *lineDst = (unsigned char *)(dstImg->imageData + dstImg->widthStep * i);
        for (int j = 0; j < srcImg->width; j++) {
            int tmp = (int)((lineSrc[j] - scMean) * contrastRatio * cn + scMean * brightRatio * bn);
            lineDst[j] = (unsigned char)(tmp > 255 ? 255 : (tmp < 0 ? 0 : tmp));
        }
    }
}

void adjContrastAndBright2(const IplImage *srcImg, IplImage *dstImg) {
    static const double kDefaultMean = 180;
    static const double kDefaultSdv = 30;
    CvSize szSrc = cvGetSize(srcImg);
    CvScalar scSampleMean;
    CvScalar scSampleSdv;
    CvRect rtSample = cvRect(szSrc.width / 2 - 100, szSrc.height / 2 - 150, 200, 300);
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

// 噪点过滤
void filterNosiyPoint(IplImage *srcImg) {
    CvMemStorage *memStorage = cvCreateMemStorage(0);
    IplImage *cpSrcImg = cvCreateImage(cvGetSize(srcImg), srcImg->depth, srcImg->nChannels);
    cvCopy(srcImg, cpSrcImg, NULL);

    CvSeq *cont = NULL;
    CvContourScanner contScanner = NULL;
    contScanner = cvStartFindContours(cpSrcImg, memStorage, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));
    while ((cont = cvFindNextContour(contScanner)) != NULL) {
        double area = cvContourArea(cont, CV_WHOLE_SEQ, 0);
        CvRect rect = cvBoundingRect(cont, 0);
        if (area < 36 || area > 1400) {
            for (int i = 0; i < rect.height; i++) {
                unsigned char *ptr = (unsigned char *)(srcImg->imageData + srcImg->widthStep * (rect.y + i));
                for (int j = 0; j < rect.width; j++) {
                    ptr[rect.x + j] = 0;
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

#define SAVE_CONTOUR_IMAGE
void filterTitleContours(IplImage *srcImg, CvSeq *results) {
    IplImage *ctImg = cvCloneImage(srcImg);
    // IplImage *ctRstImg = cvCreateImage(imgSize, srcImg->depth, 3);
    // cvSet(ctRstImg, cvScalarAll(255), NULL);
    CvMemStorage *tmpStorage = cvCreateMemStorage(0);
    CvMemStorage *memStorage = cvCreateMemStorage(0);
    CvSeq *firstContour = NULL;
    CvContourScanner contourScanner = NULL;
    contourScanner = cvStartFindContours(ctImg,
                        memStorage,
                        sizeof(CvContour),
                        CV_RETR_LIST,
                        CV_CHAIN_APPROX_SIMPLE,
                        cvPoint(0, 0));
    int count = 0;
    while ((firstContour = cvFindNextContour(contourScanner)) != NULL) {
        double tempArea = cvContourArea(firstContour, CV_WHOLE_SEQ, 0);
        CvRect rect = cvBoundingRect(firstContour, 0);
        float rectScale = rect.width / rect.height;
        // 查找标题轮廓
        if (tempArea > 7500 && tempArea <= 15000 && rectScale >= 5.0 && rectScale <= 8.0) {
            // cvDrawContours(contourImg, firstContour, CV_RGB(255, 0, 0), CV_RGB(0, 255, 0), 0, CV_FILLED, 8, cvPoint(0, 0));
            // printf("Area size : %.0lf\n", tempArea);
            count++;
            CvBox2D rt2D = cvMinAreaRect2(firstContour, tmpStorage);
            // if (rt2D.angle > 0) {
            //
            // }
            // cvDrawContours(contourImg, firstContour, CV_RGB(255, 0, 0), CV_RGB(0, 255, 0), 2, CV_FILLED, 8, cvPoint(0, 0));
            // cvDrawContours(ctRstImg, firstContour, CV_RGB(255, 0, 0), \
                            // CV_RGB(0, 255, 0), 0, 5, 8, cvPoint(0, 0));
            if (rt2D.angle )
            fprintf(stdout, "angle = %.2f， scale = %.2f\n", rt2D.angle, rt2D.size.width / rt2D.size.height);
            fprintf(stdout, "rect's scale = %.2f\n", (float)rect.width / rect.height);
            cvSeqPush(results, &rect);
            cvClearMemStorage(tmpStorage);
        }
        // 查找2轮廓
    }
    cvEndFindContours(&contourScanner);
    // char *ctImgPath = genFilePath(dstImgPath, "_ct");
    // saveImage(ctImgPath, contourImg);
    // free(ctImgPath);
    // char *ctSrcImgPath = genFilePath(dstImgPath, "_src_ct");
    // saveImage(ctSrcImgPath, srcImg);
    // free(ctSrcImgPath);
// #ifdef SAVE_CONTOUR_IMAGE
//     char *ctRstPath = genFilePath(g_srcImgPath, "_ct");
//     saveImage(ctRstPath, ctRstImg);
//     free(ctRstPath);
// #endif

    cvReleaseMemStorage(&tmpStorage);
    cvClearMemStorage(memStorage);
    cvReleaseMemStorage(&memStorage);
    // cvReleaseImage(&ctRstImg);
    cvReleaseImage(&ctImg);
}

void findoutSerialContours(const IplImage *srcImg, CvSeq *results) {
    IplImage *ctImg = cvCloneImage(srcImg);

    CvMemStorage *tmpStorage = cvCreateMemStorage(0);
    CvMemStorage *memStorage = cvCreateMemStorage(0);
    CvSeq *firstContour = NULL;
    CvContourScanner contourScanner = NULL;
    contourScanner = cvStartFindContours(ctImg,
                        memStorage,
                        sizeof(CvContour),
                        CV_RETR_LIST,
                        CV_CHAIN_APPROX_SIMPLE,
                        cvPoint(0, 0));
    while ((firstContour = cvFindNextContour(contourScanner)) != NULL) {
        double tempArea = cvContourArea(firstContour, CV_WHOLE_SEQ, 0);
        CvRect rect = cvBoundingRect(firstContour, 0);
        float rectScale = (float)rect.width / rect.height;
        // fprintf(stdout, "area = %.2f， scale = %.2f\n", tempArea, (float)rect.width / rect.height);
        if (tempArea >= MIN_SERIAL_AREA_SIZE && tempArea <= MAX_SERIAL_AREA_SIZE
            && rectScale >= MIN_SERIAL_AREA_SCALE && rectScale <= MAX_SERIAL_AREA_SCALE) {
            CvBox2D rt2D = cvMinAreaRect2(firstContour, tmpStorage);
            float angle = 0.0;
            if (rt2D.angle) {
                if (rt2D.size.width < rt2D.size.height) {
                    angle = 90 + rt2D.angle;
                } else {
                    angle = rt2D.angle;
                }
            }
            ContourRes res = {.rect = rect, .angle = angle};
            cvSeqPush(results, &res);
            cvClearMemStorage(tmpStorage);
        }
    }
    cvEndFindContours(&contourScanner);

    cvReleaseMemStorage(&tmpStorage);
    cvClearMemStorage(memStorage);
    cvReleaseMemStorage(&memStorage);
    cvReleaseImage(&ctImg);
}
