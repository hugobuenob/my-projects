#ifndef LBP
#define LBP
//#define STANDAR
//#define IMPROVEMENT
//#define IMPROVEMENT2
#define IMPROVEMENT3


#include <stdint.h>
#include <math.h>
#include <stdio.h>

#define HISTOGRAM_WIDTH 256
#define TYPES 24

void SVM(float features[HISTOGRAM_WIDTH], float mu[TYPES][HISTOGRAM_WIDTH], float sigma[TYPES][HISTOGRAM_WIDTH], 
         float bias[TYPES], float scale[TYPES], float beta[TYPES][HISTOGRAM_WIDTH], uint8_t *prediction);

#endif