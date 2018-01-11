#include "BaDetect.h"
#include "BaFileUtils.h"

void searchTitleBySobel(const IplImage *srcImg, CvSeq *results);

void searchTitleByMorph(const IplImage *srcImg, CvSeq *results) {
    IplImage *plSrcCpy = cvCloneImage(srcImg);
    CvSize szSrc = cvGetSize(srcImg);
    // CvSize szArea = cvSize(szSrc.width, szSrc.height);
    CvSize szArea = cvSize(szSrc.width / 2, szSrc.height);
    CvRect rtDetectArea = cvRect(szSrc.width / 4, 0, szArea.width, szArea.height);
    cvSetImageROI(plSrcCpy, rtDetectArea);
    IplImage *plArea = cvCreateImage(szArea, plSrcCpy->depth, plSrcCpy->nChannels);
    plArea->origin = plSrcCpy->origin;
    cvCopy(plSrcCpy, plArea, NULL);
    cvResetImageROI(plSrcCpy);
    cvReleaseImage(&plSrcCpy);
    IplImage *plTmp = cvCreateImage(szArea, plArea->depth, plArea->nChannels);
    fprintf(stdout, "area img size: [%d x %d]\n", plArea->width, plArea->height);
    morphOprTitleArea(plArea, plTmp);
    filterTitleContours(plTmp, results);
    for (int i = 0; i < results->total; i++) {
        ContourRes *pRes = (ContourRes *)cvGetSeqElem(results, i);
        cvRectangle(plArea,
            cvPoint(pRes->rect.x, pRes->rect.y),
            cvPoint(pRes->rect.x + pRes->rect.width, pRes->rect.y + pRes->rect.height),
            cvScalarAll(0), 5, 8, 0);
    }

    saveProcessImage(plArea, "_rect_in_src");

    cvReleaseImage(&plTmp);
    cvReleaseImage(&plArea);
}

void searchSerialBySobel(const IplImage *srcImg, CvSeq *results);

void searchSerialByMorph(const IplImage *srcImg, CvSeq *results) {
    IplImage *plSrcCpy = cvCloneImage(srcImg);
    CvSize szSrc = cvGetSize(srcImg);
    CvSize szArea = cvSize(szSrc.width / 2, szSrc.height / 2);
    CvRect rtROI = cvRect(szSrc.width / 2, 0, szArea.width, szArea.height);
    IplImage *plDetectArea = cvCreateImage(szArea, plSrcCpy->depth, plSrcCpy->nChannels);
    cvSetImageROI(plSrcCpy, rtROI);
    cvCopy(plSrcCpy, plDetectArea, NULL);
    cvResetImageROI(plSrcCpy);
    cvReleaseImage(&plSrcCpy);
    IplImage *plDst = cvCreateImage(szArea, plDetectArea->depth, plDetectArea->nChannels);
    morphOprSerialArea(plDetectArea, plDst);
    findoutSerialContours(plDst, results);

    for (int i = 0; i < results->total; i++) {
        ContourRes *pRes = (ContourRes *)cvGetSeqElem(results, i);
        cvRectangle(plDetectArea,
            cvPoint(pRes->rect.x, pRes->rect.y),
            cvPoint(pRes->rect.x + pRes->rect.width, pRes->rect.y + pRes->rect.height),
            cvScalarAll(0), 5, 8, 0);
        pRes->rect.x += szSrc.width / 2;
    }

    saveProcessImage(plDetectArea, "_rect_in_src");

    cvReleaseImage(&plDetectArea);
    cvReleaseImage(&plDst);
}
