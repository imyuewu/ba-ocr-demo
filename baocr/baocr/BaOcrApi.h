#ifndef __BA_OCR_API_H__
#define __BA_OCR_API_H__

#include "stdbool.h"

typedef enum ImageQuality {IMAGE_QUALITY_HIGH, IMAGE_QUALITY_MEDIAN, IMAGE_QUALITY_LOW} ImageQuality;
typedef enum OcrResultType {
    RESULT_IS_BA_IMAGE,
    RESULT_BA_SERIAL,
    RESULT_BA_EXPIRE_DATE
} OcrResultType;

typedef enum ImageOrientation {
    ORIENTATION_UP,
    ORIENTATION_DOWN,
    ORIENTATION_LEFT,
    ORIENTATION_RIGHT
} ImageOrientation;

typedef struct OcrResult {
    int errCode;
    OcrResultType resType;
    union ResData {
        bool isBAImage;
        char serial[17];
        struct ExpireData {
            char year[5];
            char month[3];
            char day[3];
        } expireData;
    } resData;
} OcrResult;

// finial goal
OcrResult isBAImage(const char *srcImgPath);
OcrResult getBASerial(const char *srcImgPath);
OcrResult getBAExpireDate(const char *srcImgPath);
void getBASerialPics(const char *srcImgPath, int size, int len, char results[size][len]);

#endif
