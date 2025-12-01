#ifndef LBP
#define LBP
//#define STANDAR
//#define IMPROVEMENT
#define IMPROVEMENT2
//#define IMPROVEMENT3


#include <stdint.h>
#include <math.h>
#include <stdio.h>

#define IMG_WIDTH 128
#define IMG_HEIGHT 128
#define HISTOGRAM_WIDTH 256
#define SPLIT 32

void LBPFeatures(uint8_t pixels[IMG_HEIGHT*IMG_WIDTH], float feature[HISTOGRAM_WIDTH]);

#endif
