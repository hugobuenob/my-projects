#ifndef LBP
#define LBP


//#define IMPROVEMENT
#define IMPROVEMENT2
//#define STANDAR

#include <stdint.h>
#include <math.h>

#define IMG_WIDTH 128
#define IMG_HEIGHT 128
#define HISTOGRAM_WIDTH 256
#define SPLIT 8

void LBPFeatures_custom(uint8_t pixels[IMG_WIDTH*IMG_WIDTH], float vector[HISTOGRAM_WIDTH]);

#endif
