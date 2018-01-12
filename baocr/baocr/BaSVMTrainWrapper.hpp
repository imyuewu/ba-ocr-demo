#ifndef __BA_SVM_TRAIN_WRAPPER_HPP__
#define __BA_SVM_TRAIN_WRAPPER_HPP__

struct tagTrain;
#ifdef __cplusplus
extern "C" {
#endif
    struct tagTrain *GetInstance2();
    struct tagTrain *GetInstance(const char *xmlPath);
    void ReleaseInstance(struct tagTrain **ppInstance);
    void train_for_c(struct tagTrain *pTrain, const char *path);
    float predict_for_c(struct tagTrain *pTrain, const char *path);
    void setXMLPath_for_c(struct tagTrain *pTrain, const char *path);
#ifdef __cplusplus
};
#endif

#endif
