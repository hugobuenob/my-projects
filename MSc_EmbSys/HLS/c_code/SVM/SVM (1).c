#include "SVM.h"

void SVM(float features[HISTOGRAM_WIDTH], float mu[TYPES][HISTOGRAM_WIDTH],
         float sigma[TYPES][HISTOGRAM_WIDTH], float bias[TYPES], 
         float scale[TYPES], float beta[TYPES][HISTOGRAM_WIDTH], 
         uint8_t *prediction){

    uint8_t k;
    uint16_t i;
    float predictors[HISTOGRAM_WIDTH];
    float b;
    float z[HISTOGRAM_WIDTH];
    float acum[TYPES] = {0};
    float predicted;
    float last_predicted = -100000;
    
    loop_histogram: for (i = 0; i < HISTOGRAM_WIDTH; i++){
        loop_types1: for (k = 0; k < TYPES; k++){
            predictors[i] = (features[i] - mu[k][i])/sigma[k][i];
            z[i] = predictors[i]/scale[k];
            acum[k] += z[i] * beta[k][i];
        }
    }

    loop_types2: for (k = 0; k < TYPES; k++){
        predicted = acum[k] + bias[k];

        if (predicted > last_predicted)
        {
            *prediction = k;
            last_predicted = predicted;
        }
    }
}