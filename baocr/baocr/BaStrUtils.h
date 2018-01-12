#ifndef __BA_STR_UTILS_H__
#define __BA_STR_UTILS_H__

#include "BaOcrHeaders.h"

#ifdef __cplusplus
extern "C" {
#endif

char *appendStrToPath(const char *path, const char *appdStr);
void appendCharsIntoFilePath(const char *srcStr, const char *insertStr, char *resultStr);
char *genFilePath(const char *filePath, const char *pathExt);

#ifdef __cplusplus
}
#endif

#endif
