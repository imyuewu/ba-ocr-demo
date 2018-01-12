#include "BaSVMTrain.hpp"
#include "BaStrUtils.h"
#include "BaFileUtils.h"
#include "BaFeature.h"
#include "stdlib.h"

BaSVMTrain::BaSVMTrain() {
    xmlPath = NULL;
    _svm = new SVM;
}

BaSVMTrain::~BaSVMTrain() {
    if (xmlPath) {
        free(xmlPath);
        xmlPath = NULL;
    }
    delete _svm;
}

void BaSVMTrain::train(const char *dirPath, const char *xmlOutPath) {
    Mat trainData;
    Mat trainResponse;
    Mat testData;
    Mat testResponse;
    fprintf(stdout, "Start to train data...\n");
    prepareTrainData(dirPath, &trainData, &trainResponse, &testData, &testResponse);

    // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
    // params.C           = 6.0;

    // Train the SVM
    CvSVM SVM;
    SVM.train(trainData, trainResponse, Mat(), Mat(), params);
    SVM.save(xmlOutPath);

    fprintf(stdout, "Begin to test classifies...\n");
    int right = 0;
    for (int i = 0; i < testData.rows; i++) {
        Mat sample = testData.row(i);
        if (SVM.predict(sample) == testResponse.at<float>(i)) {
            right++;
        }
    }
    fprintf(stdout, "The correct rate is %.1f%% in total %d test cases.\n", (float)right / testData.rows * 100, testData.rows);

}

float BaSVMTrain::predict2(const char *imagePath) {
    Mat sampleMat = genFeatureMat(imagePath);
    return _svm->predict(sampleMat);
}

float BaSVMTrain::predict(IplImage *image) {
    CvMat *mat = cvCreateMat(1, cvGetSize(image).width, CV_32FC1);
    getXMapFeature(image, mat);
    Mat sampleMat = Mat(mat, true);
    cvReleaseMat(&mat);
    return _svm->predict(sampleMat);
}

void BaSVMTrain::setXMLPath(const char *path) {
    if (xmlPath) {
        free(xmlPath);
        xmlPath = NULL;
    }
    xmlPath = (char *)calloc(strlen(path) + 1, sizeof(char));
    strncpy(xmlPath, path, strlen(path));
    _svm->load(xmlPath);
}

void BaSVMTrain::prepareTrainData(const char *docPath, Mat *trainData, Mat *trainResponse, Mat *testData, Mat *testResponse) {
    char *train1DocPath = appendStrToPath(docPath, "train/positive");
    char *train0DocPath = appendStrToPath(docPath, "train/nagetive");
    char *test1DocPath = appendStrToPath(docPath, "test/positive");
    char *test0DocPath = appendStrToPath(docPath, "test/nagetive");

    DIR *pTrain1Dir;
    DIR *pTrain0Dir;
    DIR *pTest1Dir;
    DIR *pTest0Dir;

    if ((pTrain1Dir = opendir(train1DocPath)) == NULL) {
        fprintf(stderr, "Can't open dir %s.", train1DocPath);
        return;
    }
    if ((pTrain0Dir = opendir(train0DocPath)) == NULL) {
        fprintf(stderr, "Can't open dir %s.", train0DocPath);
        return;
    }
    if ((pTest1Dir = opendir(test1DocPath)) == NULL) {
        fprintf(stderr, "Can't open dir %s.", test1DocPath);
        return;
    }
    if ((pTest0Dir = opendir(test0DocPath)) == NULL) {
        fprintf(stderr, "Can't open dir %s.", test0DocPath);
        return;
    }

    struct dirent *pFileInDir;
    trainData->release();
    trainResponse->release();

    while ((pFileInDir = readdir(pTrain1Dir)) != NULL) {
        if ((pFileInDir->d_type & DT_REG) && isImageFile(pFileInDir->d_name)) { // 普通文件
            char *fileFullPath = appendStrToPath(train1DocPath, pFileInDir->d_name);
            trainData->push_back(genFeatureMat(fileFullPath));
            trainResponse->push_back((float)1.0);
            free(fileFullPath);
        }
    }
    closedir(pTrain1Dir);
    free(train1DocPath);

    while ((pFileInDir = readdir(pTrain0Dir)) != NULL) {
        if ((pFileInDir->d_type & DT_REG) && isImageFile(pFileInDir->d_name)) { // 普通文件
            char *fileFullPath = appendStrToPath(train0DocPath, pFileInDir->d_name);
            trainData->push_back(genFeatureMat(fileFullPath));
            trainResponse->push_back((float)-1.0);
            free(fileFullPath);
        }
    }
    closedir(pTrain0Dir);
    free(train0DocPath);
    fprintf(stdout, "traindata rows = %d, cols = %d\n", trainData->rows, trainData->cols);

    testData->release();
    testResponse->release();
    while ((pFileInDir = readdir(pTest1Dir)) != NULL) {
        if ((pFileInDir->d_type & DT_REG) && isImageFile(pFileInDir->d_name)) { // 普通文件
            char *fileFullPath = appendStrToPath(test1DocPath, pFileInDir->d_name);
            testData->push_back(genFeatureMat(fileFullPath));
            testResponse->push_back((float)1.0);
            free(fileFullPath);
        }
    }
    closedir(pTest1Dir);
    free(test1DocPath);


    while ((pFileInDir = readdir(pTest0Dir)) != NULL) {
        if ((pFileInDir->d_type & DT_REG) && isImageFile(pFileInDir->d_name)) { // 普通文件
            char *fileFullPath = appendStrToPath(test0DocPath, pFileInDir->d_name);
            testData->push_back(genFeatureMat(fileFullPath));
            testResponse->push_back((float)-1.0);
            free(fileFullPath);
        }
    }
    closedir(pTest0Dir);
    free(test0DocPath);
    fprintf(stdout, "tessdata rows = %d, cols = %d\n", testData->rows, testData->cols);
}

Mat BaSVMTrain::genFeatureMat(const char *imagePath) {
    IplImage *image = cvLoadImage(imagePath, CV_LOAD_IMAGE_UNCHANGED);
    CvMat *mat = cvCreateMat(1, cvGetSize(image).width, CV_32FC1);
    getXMapFeature(image, mat);
    Mat result = Mat(mat, true);
    cvReleaseImage(&image);
    cvReleaseMat(&mat);
    return result;
}
