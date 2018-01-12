#ifndef __BA_SVM_TRAIN_HPP__
#define __BA_SVM_TRAIN_HPP__
#include <opencv2/ml.hpp>

using namespace std;
using namespace cv;

class BaSVMTrain {
    const char *xmlPath;
    SVM *_svm;
public:
    BaSVMTrain();
    BaSVMTrain(const char *path);
    ~BaSVMTrain();
    void setXMLPath(const char *path);
    void train(const char *path);
    float predict(const char *imagePath);
private:
    void prepareTrainData(const char *docPath, Mat *trainData, Mat *trainResponse, Mat *testData, Mat *testResponse);

    Mat genFeatureMat(const char *imagePath);
};

#endif
