#include "BaFileUtils.h"
#include "BaAlgo.h"
#include "BaStrUtils.h"

const char *g_srcImgPath = "a";

static void SaveImageJPG(const char *filePath, IplImage *img);
static void SaveImagePNG(const char *filePath, IplImage *img);

void saveImage(const char *filePath, IplImage *img) {
    if (strstr(filePath, "png") || strstr(filePath, "PNG")) {
        SaveImagePNG(filePath, img);
    } else if (strstr(filePath, "jpg") || strstr(filePath, "JPG")) {
        SaveImageJPG(filePath, img);
    } else {
        SaveImageJPG(filePath, img);
    }
}

void saveProcessImage(IplImage *img, const char *ext) {
    char *path = genFilePath(g_srcImgPath, ext);
    saveImage(path, img);
    free(path);
}

static void SaveImageJPG(const char *filePath, IplImage *img) {
    int writeParams[] = {CV_IMWRITE_JPEG_QUALITY, 95};
    cvSaveImage(filePath, img, writeParams);
}

static void SaveImagePNG(const char *filePath, IplImage *img) {
    int writeParams[] = {CV_IMWRITE_PNG_COMPRESSION, 3};
    cvSaveImage(filePath, img, writeParams);
}


void genGrayHistImage(const char *outImgPath, const CvHistogram *hist, CvSize srcImgSize) {
    // int scale = 2;
    const int kOutHeight = GRAY_LEVELS;
    const int kOutWidth = GRAY_LEVELS;
    IplImage *outImg = cvCreateImage(cvSize(kOutWidth, kOutHeight / 2), 8, 1);
    cvSetZero(outImg);

    // draw gray histogram
    float maxValue, minValue;
    int maxValueIdx, minValueIdx;
    cvGetMinMaxHistValue(hist, &minValue, &maxValue, &minValueIdx, &maxValueIdx);
    // 直方图归一化处理
    // cvNormalizeHist(grayHist, 1);
    const int kOutMaxThreshold = (srcImgSize.width * srcImgSize.height) * ((float) 6 / GRAY_LEVELS);
    float dividend = maxValue;
    bool hasTooBig = maxValue >= kOutMaxThreshold ? true : false;
    if (hasTooBig) dividend = kOutMaxThreshold;
    // unsigned long total = 0;
    for (int i = 0; i < hist->mat.dim[0].size; i++) {
        float binValue = cvQueryHistValue_1D(hist, i);
        // printf("binValue[%d] = %.0f\n", i, binValue);
        // total += binValue;
        if (hasTooBig && binValue > kOutMaxThreshold) binValue = kOutMaxThreshold;
        int intensity = cvRound(binValue * outImg->height / dividend);  //要绘制的高度
        // int intensity = binValue * outImg->height * 0.9 / dividend;  //要绘制的高度

        // CvPoint startPoint = cvPoint(i * scale, outImg->height - 1);
        // CvPoint endPoint = cvPoint((i + 1) * scale, outImg->height - intensity);
        CvPoint startPoint = cvPoint(i, outImg->height);
        CvPoint endPoint = cvPoint(i, outImg->height - intensity);
        cvLine(outImg, startPoint, endPoint, cvScalarAll(255), 1, 8, 0);
    }

    // fprintf(stdout, "%s: maxValue = %.0f, minValue = %.0f, maxIdx = %d, minIdx = %d\n", __FUNCTION__, maxValue, minValue, maxValueIdx, minValueIdx);
    saveImage(outImgPath, outImg);
    cvReleaseImage(&outImg);
}

void genBinMap(const char *imgPath, const char *outImgPath) {
    IplImage *srcImg = cvLoadImage(imgPath, CV_LOAD_IMAGE_GRAYSCALE);
    IplImage *outImg = cvCreateImage(cvGetSize(srcImg), 8, 1);
    cvSetZero(outImg);
    for (int i = 0; i < srcImg->height; i++) {
        unsigned char *linePtr = (unsigned char *)(srcImg->imageData + i * srcImg->widthStep);
        unsigned char *outLinePtr = (unsigned char *)(outImg->imageData + i * srcImg->widthStep);
        for (int j = 0; j < srcImg->width; j++) {
            if (*linePtr == 255) {
                *outLinePtr = 255;
                outLinePtr++;
            }
            linePtr++;
        }
    }

    saveImage(outImgPath, outImg);
    cvReleaseImage(&srcImg);
    cvReleaseImage(&outImg);
}


void genGrayHistImageByFile(const char *imgPath, const char *outImgPath) {
    IplImage *srcImg = cvLoadImage(imgPath, CV_LOAD_IMAGE_GRAYSCALE);

    // init histogram
    CvHistogram *grayHist = NULL;
    initGrayHist(&grayHist);

    // auto calculate histogram
    IplImage *images[] = {srcImg};
    cvCalcHist(images, grayHist, 0, NULL);

    genGrayHistImage(outImgPath, grayHist, cvGetSize(srcImg));

    cvReleaseHist(&grayHist);
    cvReleaseImage(&srcImg);
}

bool isImageFile(const char *filePath) {
    char ImageExtStr[] = IMAGE_TYPE_SUPPORT_STR;
    static const char tokseps[] = "|";
    bool result = false;
    char fileExt[16] = "\0";
    getFileExt(filePath, fileExt);
    // fprintf(stdout, "file ext: %s\n", fileExt);
    if (strlen(fileExt) <= 0) return result;
    char *pStr;
    pStr = strtok(ImageExtStr, tokseps);
    while (pStr) {
        // fprintf(stdout, "pStr = %s\n", pStr);
        if (strcmp(fileExt, pStr) == 0) {
            result = true;
            break;
        }
        pStr = strtok(NULL, tokseps);
    }
    return result;
}

bool isFileExists(const char *filePath) {
    bool result = false;
    FILE *pFile = NULL;
    if ((pFile = fopen(filePath, "r")) != NULL) {
        result = true;
        fclose(pFile);
    }
    
    return result;
}

bool isDirectory(const char *dirPath) {
    bool result = false;
    
    DIR *pDir;
    if ((pDir = opendir(dirPath)) != NULL) {
        result = true;
        closedir(pDir);
    }
    
    return result;
}

void getFileExt(const char *filePath, char *fileExt) {
    char *pStr = NULL;
    if ((pStr = strrchr(filePath, '.')) != NULL) {
        strncpy(fileExt, pStr, strlen(pStr) + 1);
    }
}
