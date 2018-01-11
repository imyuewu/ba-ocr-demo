#ifndef __BA_TYPE_H__
#define __BA_TYPE_H__

#include <stdbool.h>
#include "BaOcrErrorCode.h"

typedef enum ImageQuality {IMAGE_QUALITY_HIGH, IMAGE_QUALITY_MEDIAN, IMAGE_QUALITY_LOW} ImageQuality;
typedef enum OcrResultType {
    RESULT_IS_BA_IMAGE,
    RESULT_BA_SERIAL,
    RESULT_BA_EXPIRE_DATE
} OcrResultType;

typedef enum BAImageOrientation {
    ORIENTATION_UP,
    ORIENTATION_DOWN,
    ORIENTATION_LEFT,
    ORIENTATION_RIGHT
} BAImageOrientation;

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

#endif
