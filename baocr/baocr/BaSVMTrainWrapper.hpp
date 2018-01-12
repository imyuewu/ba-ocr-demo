#ifndef __BA_SVM_TRAIN_WRAPPER_HPP__
#define __BA_SVM_TRAIN_WRAPPER_HPP__
#include <opencv2/ml.hpp>
struct tagTrain;
#ifdef __cplusplus
extern "C" {
#endif
    struct tagTrain *GetInstance();
    void ReleaseInstance(struct tagTrain **ppInstance);
    void Train(struct tagTrain *pTrain, const char *imgDir, const char *xmlOutPath);
    float Predict(struct tagTrain *pTrain, IplImage *image);
    float Predict2(struct tagTrain *pTrain, const char *path);
    void SetXMLPath(struct tagTrain *pTrain, const char *path);
#ifdef __cplusplus
};
#endif

#endif
