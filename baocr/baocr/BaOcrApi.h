#ifndef __BA_OCR_API_H__
#define __BA_OCR_API_H__

#include "BaType.h"

// finial goal
OcrResult isBAImage(const char *srcImgPath);
OcrResult getBASerial(const char *srcImgPath);
OcrResult getBAExpireDate(const char *srcImgPath);
void getBASerialPics(const char *srcImgPath, int size, int len, char results[size][len]);

#endif
