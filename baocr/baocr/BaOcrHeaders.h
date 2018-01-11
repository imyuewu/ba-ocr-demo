#ifndef __BA_OCR_HEADERS_H__
#define __BA_OCR_HEADERS_H__
// std unix c
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>

// opencv
#include <opencv2/core/core_c.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/highgui.hpp>
#include <opencv2/legacy/compat.hpp>
#include <opencv2/ml.hpp>

// user config
#include "BaOcrErrorCode.h"

#ifndef DEBUG
#define DEBUG
#endif
#ifdef RELEASE
#undef DEBUG
#endif

#define IMAGE_TYPE_SUPPORT_STR ".jpg|.jpeg|.png|.JPG|.JPEG|.PNG"

#define MIN_BA_AREA_RATIO 0.65
#define STD_WIDTH 1000
#define GRAY_LEVELS 256
#define MIN_SERIAL_AREA_SIZE 1500
#define MAX_SERIAL_AREA_SIZE 4500
#define MIN_SERIAL_AREA_SCALE 5.0
#define MAX_SERIAL_AREA_SCALE 10.0


#endif
