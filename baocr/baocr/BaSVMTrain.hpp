#ifndef __BA_SVM_TRAIN_HPP__
#define __BA_SVM_TRAIN_HPP__

extern "C" void initSVM(const char *xmlFilePath);
extern "C" void train(const char *path);
extern "C" int predict(const char *imagePath);

#endif
