#include "BaSVMTrainWrapper.hpp"
#include "BaSVMTrain.hpp"

#ifdef __cplusplus
extern "C" {
#endif
struct tagTrain {
    BaSVMTrain svmTrain;
};

struct tagTrain *GetInstance() {
    return new struct tagTrain;
}

void ReleaseInstance(struct tagTrain **ppInstance) {
    delete *ppInstance;
    *ppInstance = 0;
}

void Train(struct tagTrain *pTrain, const char *imgDir, const char *xmlOutPath) {
    pTrain->svmTrain.train(imgDir, xmlOutPath);
}

float Predict(struct tagTrain *pTrain, IplImage *image) {
    return pTrain->svmTrain.predict(image);
}
    
float Predict2(struct tagTrain *pTrain, const char *path) {
    return pTrain->svmTrain.predict2(path);
}

void SetXMLPath(struct tagTrain *pTrain, const char *path) {
    pTrain->svmTrain.setXMLPath(path);
}

#ifdef __cplusplus
};
#endif
