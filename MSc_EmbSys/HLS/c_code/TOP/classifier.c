#include "classifier.h"

float fast_sqrt(float number) {

    union {
        float f;
        uint32_t i;
    } conv = { number };

    conv.i = 0x1fbd1df5 + (conv.i >> 1);        // bit hack 1/2log(x)
    conv.f = 0.5f * (conv.f + number / conv.f); // newton raphson method iteration 1
    //conv.f = 0.5f * (conv.f + number / conv.f); // newton raphson method iteration 2

    return conv.f;
}

void extract_histogram(uint8_t pixels[IMG_HEIGHT * IMG_WIDTH / SPLIT + 2 * IMG_WIDTH], uint16_t histogram[HISTOGRAM_WIDTH], uint8_t length){

    uint8_t transform;
    uint8_t i, j, kk;

    loop_height: for (i = 1; i < length; i++){
        loop_with: for (j = 1; j < IMG_WIDTH - 1; j++){

            transform = 0;
            loop_neighbourhood: for(kk = 0; kk < 8; kk++){
                int8_t row_offset = (kk == 1 || kk == 2 || kk == 3) ? -1 : (kk == 5 || kk == 6 || kk == 7) ? 1 : 0;
                int8_t col_offset = (kk == 3 || kk == 4 || kk == 5) ? -1 : (kk == 7 || kk == 0 || kk == 1) ? 1 : 0;

                transform += (pixels[i * IMG_WIDTH + j] < pixels[(i + row_offset) * IMG_WIDTH + (j + col_offset)]) ? (1 << kk) : 0;

            }

            histogram[transform]++;
        }
    }
}

void LBPFeatures(uint8_t pixels[IMG_HEIGHT*IMG_WIDTH], float feature[HISTOGRAM_WIDTH]){

    uint16_t i;
    uint8_t j;

    uint16_t histogram[SPLIT][HISTOGRAM_WIDTH];
    uint8_t slice_pixels[SPLIT][IMG_HEIGHT*IMG_WIDTH/SPLIT+2*IMG_WIDTH];
    uint32_t histogram_product[HISTOGRAM_WIDTH];
    uint32_t acum = 0;

    loop_ini_hist2: for (i = 0; i < HISTOGRAM_WIDTH; i++) {
    	loop_ini_hist1: for (j = 0; j < SPLIT; j++) {
    		histogram[j][i] = 0;
        }
    }

    uint16_t slice_size = IMG_HEIGHT * IMG_WIDTH / SPLIT;
    uint16_t extended_slice_size = slice_size + 2 * IMG_WIDTH;

    loop_split2: for (i = 0; i < extended_slice_size; i++) {
        loop_split1: for (j = 0; j < SPLIT; j++) {
            if (i < (j == SPLIT - 1) ? slice_size : extended_slice_size) {
                slice_pixels[j][i] = pixels[i + j * slice_size];
            }
        }
    }

    loop_process: for (j = 0; j < SPLIT; j++) {
        extract_histogram(slice_pixels[j], histogram[j], (j == SPLIT - 1) ? (IMG_HEIGHT / SPLIT - 1) : (IMG_HEIGHT / SPLIT + 1));
    }

    uint16_t temp;
    loop_gather2: for (i = 0; i < HISTOGRAM_WIDTH; i++) {
        loop_gather1: for (j = 0; j < SPLIT; j++) {
             temp = (j == 0) ? histogram[0][i] : temp + histogram[j][i];
        }
        histogram[0][i] = temp;
        histogram_product[i] = histogram[0][i] * histogram[0][i];
    	acum += histogram_product[i];
    }

    float norm = fast_sqrt(acum);

    loop_division: for (i = 0; i < HISTOGRAM_WIDTH; i++) {
        feature[i] = histogram[0][i] / norm;
    }

}

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

void classifier(uint8_t pixels[IMG_HEIGHT*IMG_WIDTH], float mu[TYPES][HISTOGRAM_WIDTH],
         float sigma[TYPES][HISTOGRAM_WIDTH], float bias[TYPES], 
         float scale[TYPES], float beta[TYPES][HISTOGRAM_WIDTH], 
         uint8_t *prediction){

    float features[HISTOGRAM_WIDTH];

    LBPFeatures( pixels, features);

    SVM(features, mu, sigma, bias, scale, beta, prediction);

}
