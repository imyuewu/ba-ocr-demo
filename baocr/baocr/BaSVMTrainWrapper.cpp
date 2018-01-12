#include "BaSVMTrainWrapper.hpp"
#include "BaSVMTrain.hpp"

#ifdef __cplusplus
extern "C" {
#endif
struct tagTrain {
    BaSVMTrain svmTrain;
public:
    tagTrain() {}
    tagTrain(const char *xmlPath) {
        svmTrain = BaSVMTrain(xmlPath);
    }
};

struct tagTrain *GetInstance2() {
    return new struct tagTrain();
}

struct tagTrain *GetInstance(const char *xmlPath) {
    return new struct tagTrain(xmlPath);
}

void ReleaseInstance(struct tagTrain **ppInstance) {
    delete *ppInstance;
    *ppInstance = 0;
}

void train_for_c(struct tagTrain *pTrain, const char *path) {
    pTrain->svmTrain.train(path);
}

float predict_for_c(struct tagTrain *pTrain, const char *path) {
    return pTrain->svmTrain.predict(path);
}

void setXMLPath_for_c(struct tagTrain *pTrain, const char *path) {
    pTrain->svmTrain.setXMLPath(path);
}

#ifdef __cplusplus
};
#endif
