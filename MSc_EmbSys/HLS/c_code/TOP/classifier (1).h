#ifndef CASSIFIER
#define CASSIFIER

#include <stdint.h>

#define IMG_WIDTH 128
#define IMG_HEIGHT 128
#define HISTOGRAM_WIDTH 256
#define SPLIT 32
#define TYPES 24

void classifier(uint8_t pixels[IMG_HEIGHT*IMG_WIDTH], float mu[TYPES][HISTOGRAM_WIDTH],
         float sigma[TYPES][HISTOGRAM_WIDTH], float bias[TYPES], 
         float scale[TYPES], float beta[TYPES][HISTOGRAM_WIDTH], 
         uint8_t *prediction);

#endif