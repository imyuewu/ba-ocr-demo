#include "BaAlgo.h"

void initGrayHist(CvHistogram **hist) {
    int histSize[] = {GRAY_LEVELS};
    float histRange[] = {0, 256}; // 这里要注意被坑了，实际计算取值范围为[0, 256)即[0, 255]
    float *ranges[] = {histRange};
    *hist = cvCreateHist(1, histSize, CV_HIST_ARRAY, ranges, 1);
}

int calOtsuThreshlod(CvHistogram *hist) {
    const int kLevels = 256;
    const int kDefaultThresholdValue = 140;
    int thresholdValue = 0;

    int n1, n2, n3, minValue, maxValue, maxIdx, minIdx;
    double m1, m2, sum1, sum2, f_max, f_sb;

    n1 = n2 = n3 = 0, minValue = kLevels - 1, maxValue = 0, maxIdx = minIdx = 0;
    sum1 = sum2 = 0.0, f_max = -1.0;

    for (int i = 0; i < kLevels; i++) {
        float binValue = cvQueryHistValue_1D(hist, i);
        sum1 += i * binValue; // x * f(x) 质量矩
        n1 += binValue;
        if (binValue > maxValue) maxValue = binValue, maxIdx = i;
        if (binValue < minValue) minValue = binValue, minIdx = i;
    }

    if (n1 <= 0) {
        thresholdValue = kDefaultThresholdValue;
        // if n has no value, there is problems
        fprintf(stderr, "Calculate gray histogram failed! Use default thresholdValue [%d]\n", thresholdValue);
        exit(1);
    }

    for (int i = 0; i < kLevels; i++) {
        float binValue = cvQueryHistValue_1D(hist, i);
        n2 += binValue;
        if (n2 == 0) continue;
        n3 = n1 - n2;
        if (n3 == 0) break;
        sum2 += i * binValue;
        m1 = sum2 / n2;
        m2 = (sum1 - sum2) / n3;
        f_sb = n2 * n3 * (m1 - m2) * (m1 - m2);

        if (f_sb > f_max) {
            f_max = f_sb;
            thresholdValue = i;
        }
    }
    // fprintf(stdout, "%s: thresholdValue = %d, maxValue = %d, ", __FUNCTION__, thresholdValue, maxValue);
    // fprintf(stdout, "minValue = %d, maxIdx = %d, minIdx = %d\n", minValue, maxIdx, minIdx);

    return thresholdValue;
}

float calLineEntropy(int hist[], int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += hist[i];
    }

    float entropy = 0;
    for (int i = 0; i < size; i++) {
        int binValue = hist[i];
        if (binValue <= 0) continue;

        float ratio = binValue / sum;
        entropy += -ratio * logf(ratio);
    }
    return entropy;
}

float calCurrentEntropy(CvHistogram *hist, int threshold) {
    float *ptr = (float *)(hist->mat.data.fl);
    float bgSum = 0, fgSum = 0;
    for (int i = 0; i < hist->mat.dim[0].size; i++) {
        float binValue = *(ptr + i);
        if (i < threshold) {
            bgSum += binValue;
        } else {
            fgSum += binValue;
        }
    }

    float bgEntropy = 0, fgEntropy = 0;
    for (int i = 0; i < hist->mat.dim[0].size; i++) {
        float binValue = *(ptr + i);
        if (binValue <= 0) continue;

        if (i < threshold) {
            float bgRatio = binValue / bgSum;   // 熵的定义公式
            bgEntropy += -bgRatio * logf(bgRatio);
        } else {
            float fgRatio = binValue / fgSum;
            fgEntropy += -fgRatio * logf(fgRatio);
        }
    }
    return bgEntropy + fgEntropy;
}

float calMaximumEntropy(CvHistogram *hist) {
    float maxEntropy = -1.0;
    int maxEntropyIdx = -1;

    for (int i = 0; i < hist->mat.dim[0].size; i++) {
        float entrogy = calCurrentEntropy(hist, i);
        if (entrogy > maxEntropy) {
            maxEntropy = entrogy;
            maxEntropyIdx = i;
        }
    }
    return maxEntropy;
}

int calMaximumEntropyThreshold(CvHistogram *hist) {
    float maxEntropy = -1.0;
    int maxEntropyIdx = -1;

    for (int i = 0; i < hist->mat.dim[0].size; i++) {
        float entrogy = calCurrentEntropy(hist, i);
        if (entrogy > maxEntropy) {
            maxEntropy = entrogy;
            maxEntropyIdx = i;
        }
    }
    return maxEntropyIdx;
}

static const unsigned int g_meanIterationFlag = 5;
int calMeanIterationThreshold(IplImage *srcImg, int initThreshold) {
    int threshold = 0;  // 目标阈值
    unsigned int bgMeans = 0;    // 背景灰度均值
    unsigned int bgPixels = 0;   // 背景像素个数
    unsigned int fgMeans = 0;    // 前景灰度均值
    unsigned int fgPixels = 0;   // 前景像素个数

    int count = 0;
    while (abs(initThreshold - threshold) > g_meanIterationFlag) {
        count++;
        printf("loop %d, src = %d, tag = %d\n", count, initThreshold, threshold);
        threshold = initThreshold;
        for (int i = 0; i < srcImg->height; i++) {
            unsigned char *line = (unsigned char *)(srcImg->imageData + srcImg->widthStep * i);
            for (int j = 0; j < srcImg->width; j++) {
                unsigned char value = *(line + j);
                if (value < threshold) {
                    bgPixels++;
                    bgMeans += value;
                } else {
                    fgPixels++;
                    fgMeans += value;
                }
            }
        }
        if (bgPixels == 0 || fgPixels == 0) continue;

        threshold = (bgMeans / bgPixels + fgMeans / fgPixels) / 2;
    }
    return threshold;
}
