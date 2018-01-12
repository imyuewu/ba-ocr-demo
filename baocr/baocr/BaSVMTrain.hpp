#ifndef __BA_SVM_TRAIN_HPP__
#define __BA_SVM_TRAIN_HPP__
#include <opencv2/ml.hpp>
#include "string.h"

using namespace std;
using namespace cv;

class BaSVMTrain {
    char *xmlPath;
    SVM *_svm;
public:
    BaSVMTrain();
    ~BaSVMTrain();
    void setXMLPath(const char *path);
    void train(const char *dirPath, const char *xmlOutPath);
    float predict(IplImage *image);
    float predict2(const char *imagePath);
private:
    void prepareTrainData(const char *docPath, Mat *trainData, Mat *trainResponse, Mat *testData, Mat *testResponse);

    Mat genFeatureMat(const char *imagePath);
};

#endif
