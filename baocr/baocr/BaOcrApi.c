#include "BaOcrApi.h"
#include "BaDetect.h"
#include "BaCharOpr.h"
#include "BaFileUtils.h"
#include "BaImgOpr.h"
#include "BaImgUtils.h"

static int g_errorCode = NO_ERROR;

// static int initVariables(const ImageQuality imgQuality);

OcrResult isBAImage(const char *srcImgPath) {
    g_errorCode = NO_ERROR;
    g_srcImgPath = srcImgPath;
    OcrResult result;
    result.resType = RESULT_IS_BA_IMAGE;

    IplImage *plSrcImg = cvLoadImage(srcImgPath, CV_LOAD_IMAGE_COLOR);
    // get red channel
    IplImage *plRChannelImg = cvCreateImage(cvGetSize(plSrcImg), plSrcImg->depth, 1);
    getRedChannelImage(plSrcImg, plRChannelImg);
    saveProcessImage(plRChannelImg, "_r");
    cvReleaseImage(&plSrcImg);

    if ((g_errorCode = preProcImg(&plRChannelImg)) != NO_ERROR) {
        result.errCode = g_errorCode;
        return result;
    }

    CvMemStorage *memStorage = cvCreateMemStorage(0);
    CvSeq *titles = cvCreateSeq(0, sizeof(CvSeq), sizeof(ContourRes), memStorage);
    searchTitleByMorph(plRChannelImg, titles);
    for (int i = 0; i < titles->total; i++) {
        // CvBox2D *pRect = (CvBox2D *)cvGetSeqElem(results, i);
        ContourRes *pRes = (ContourRes *)cvGetSeqElem(titles, i);
        cvSetImageROI(plRChannelImg, pRes->rect);
        if (i == 0) {
            saveProcessImage(plRChannelImg, "0");
        } else if (i == 1) {
            saveProcessImage(plRChannelImg, "1");
        } else {
            saveProcessImage(plRChannelImg, "2");
        }

        cvResetImageROI(plRChannelImg);
        // cvSetImageROI(plArea, pRect, NULL);
        // cvRectangle(plArea, cvPoint(pRect->x, pRect->y), cvPoint(pRect->x + pRect->width, pRect->y + pRect->height), CV_RGB(255, 0, 0), 5, 8, 0);
    }

    cvClearMemStorage(memStorage);
    cvReleaseMemStorage(&memStorage);
    cvReleaseImage(&plRChannelImg);

    result.errCode = g_errorCode;
    return result;
}

OcrResult getBASerial(const char *srcImgPath) {
    g_errorCode = NO_ERROR;
    g_srcImgPath = srcImgPath;
    OcrResult result;
    result.resType = RESULT_BA_SERIAL;

    IplImage *plSrcImg = cvLoadImage(srcImgPath, CV_LOAD_IMAGE_COLOR);
    CvSize srcSize = cvGetSize(plSrcImg);
    // get red channel
    IplImage *plRChannelImg = cvCreateImage(srcSize, plSrcImg->depth, 1);
    getRedChannelImage(plSrcImg, plRChannelImg);
    // cvReleaseImage(&plSrcImg);

    // preprocess image
    if ((g_errorCode = preProcImg(&plRChannelImg)) != NO_ERROR) {
        result.errCode = g_errorCode;
        return result;
    }

    CvMemStorage *memStorage = cvCreateMemStorage(0);
    CvSeq *serials = cvCreateSeq(0, sizeof(CvSeq), sizeof(ContourRes), memStorage);
    searchSerialByMorph(plRChannelImg, serials);
    for (int i = 0; i < serials->total; i++) {
        // CvBox2D *pRect = (CvBox2D *)cvGetSeqElem(results, i);
        ContourRes *pRes = (ContourRes *)cvGetSeqElem(serials, i);
        cvSetImageROI(plRChannelImg, pRes->rect);
        int sz = snprintf(NULL, 0, "_serial_%d", i);
        char serialFileName[sz + 1];
        snprintf(serialFileName, sizeof serialFileName, "_serial_%d", i);
        CvSize szTmp = cvSize(pRes->rect.width, pRes->rect.height);
        IplImage *plSerial = cvCreateImage(szTmp, plRChannelImg->depth, plRChannelImg->nChannels);
        cvCopy(plRChannelImg, plSerial, NULL);
        cvResetImageROI(plRChannelImg);
        // rotate image
        if (pRes->angle != 0) {
            fprintf(stdout, "need adj angle = %.2lf\n", pRes->angle);
            IplImage *plSerialRo = cvCreateImage(cvGetSize(plSerial), plSerial->depth, plSerial->nChannels);
            rotateImg(plSerial, plSerialRo, pRes->angle);
            cvReleaseImage(&plSerial);
            plSerial = plSerialRo;
            plSerialRo = NULL;
        }
        saveProcessImage(plSerial, serialFileName);
        cvReleaseImage(&plSerial);
    }

    cvClearMemStorage(memStorage);
    cvReleaseMemStorage(&memStorage);
    cvReleaseImage(&plSrcImg);
    cvReleaseImage(&plRChannelImg);
    // cvReleaseImage(&tmpImg);
    result.errCode = g_errorCode;
    return result;
}

OcrResult getBAExpireDate(const char *srcImgPath) {
    OcrResult result;

    result.resType = RESULT_BA_EXPIRE_DATE;
    result.errCode = g_errorCode;
    return result;
}

void getBASerialPics(const char *srcImgPath, int size, int len, char results[size][len]) {
    g_srcImgPath = srcImgPath;

    IplImage *plSrcImg = cvLoadImage(srcImgPath, CV_LOAD_IMAGE_COLOR);
    CvSize srcSize = cvGetSize(plSrcImg);
    // get red channel
    IplImage *plRChannelImg = cvCreateImage(srcSize, plSrcImg->depth, 1);
    getRedChannelImage(plSrcImg, plRChannelImg);
    // cvReleaseImage(&plSrcImg);

    // preprocess image
    if ((g_errorCode = preProcImg(&plRChannelImg)) != NO_ERROR) return;

    CvMemStorage *memStorage = cvCreateMemStorage(0);
    CvSeq *serials = cvCreateSeq(0, sizeof(CvSeq), sizeof(ContourRes), memStorage);
    searchSerialByMorph(plRChannelImg, serials);
    for (int i = 0; i < serials->total; i++) {
        // CvBox2D *pRect = (CvBox2D *)cvGetSeqElem(results, i);
        ContourRes *pRes = (ContourRes *)cvGetSeqElem(serials, i);
        cvSetImageROI(plRChannelImg, pRes->rect);
        int sz = snprintf(NULL, 0, "_serial_%d", i);
        char serialFileName[sz + 1];
        snprintf(serialFileName, sizeof serialFileName, "_serial_%d", i);
        CvSize szTmp = cvSize(pRes->rect.width, pRes->rect.height);
        IplImage *plSerial = cvCreateImage(szTmp, plRChannelImg->depth, plRChannelImg->nChannels);
        cvCopy(plRChannelImg, plSerial, NULL);
        cvResetImageROI(plRChannelImg);
        // rotate image
        if (pRes->angle != 0) {
            fprintf(stdout, "need adj angle = %.2lf\n", pRes->angle);
            IplImage *plSerialRo = cvCreateImage(cvGetSize(plSerial), plSerial->depth, plSerial->nChannels);
            rotateImg(plSerial, plSerialRo, pRes->angle);
            cvReleaseImage(&plSerial);
            plSerial = plSerialRo;
            plSerialRo = NULL;
        }
        char *fullPath = genFilePath(srcImgPath, serialFileName);
        strncpy(results[i], fullPath, strlen(fullPath) + 1);
        free(fullPath);
        oprSerialImage(plSerial, plSerial);
        saveProcessImage(plSerial, serialFileName);
        cvReleaseImage(&plSerial);
    }

    cvClearMemStorage(memStorage);
    cvReleaseMemStorage(&memStorage);
    cvReleaseImage(&plSrcImg);
    cvReleaseImage(&plRChannelImg);
}
