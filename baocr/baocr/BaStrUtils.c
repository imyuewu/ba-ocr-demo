#include "BaStrUtils.h"

char *appendStrToPath(const char *path, const char *appdStr) {
    const char *fmt = "%s/%s";
    int sz = snprintf(NULL, 0, fmt, path, appdStr) + 1;
    char *result = calloc(sz, sizeof(char));
    snprintf(result, sz, fmt, path, appdStr);
    return result;
}

void appendCharsIntoFilePath(const char *srcStr, const char *insertStr, char *resultStr) {
    char c = '.';
    char *dotPtr = NULL;
    strncpy(resultStr, srcStr, strlen(srcStr));
    resultStr[strlen(srcStr)] = '\0';
    if ((dotPtr = strchr(srcStr, c)) == NULL) {
        strncat(resultStr, insertStr, strlen(insertStr));
    } else {
        long charsBeforeDot = dotPtr - srcStr;
        // printf("charsBeforeDot len : %d\n", charsBeforeDot);
        resultStr[charsBeforeDot] = '\0';
        strncat(resultStr, insertStr, strlen(insertStr));
        strncat(resultStr, dotPtr, strlen(dotPtr));
    }
}


char *genFilePath(const char *filePath, const char *pathExt) {
    unsigned long len = strlen(filePath) + strlen(pathExt) + 1;
    char *finalPath = malloc(sizeof(char) * len);
    appendCharsIntoFilePath(filePath, pathExt, finalPath);
    return finalPath;
}
