#include "LBPFeatures.h"

#ifdef STANDAR

void LBPFeatures(uint8_t pixels[IMG_HEIGHT*IMG_WIDTH], float feature[HISTOGRAM_WIDTH]){

    int i, j, kk;
    uint8_t neighbourhood[8];
    uint8_t transform;
    uint16_t histogram[HISTOGRAM_WIDTH] = {0};

    loop_height: for (i = 1; i < IMG_HEIGHT - 1; i++){
    	loop_with: for (j = 1; j < IMG_WIDTH - 1; j++){


            //Neighbourhood =[I(i,j+1) I(i-1,j+1) I(i-1,j) I(i-1,j-1) I(i,j-1) I(i+1,j-1) I(i+1,j) I(i+1,j+1)];
            neighbourhood[0] = pixels[i * IMG_WIDTH + j + 1];  
            neighbourhood[1] = pixels[(i - 1) * IMG_WIDTH + j + 1];
            neighbourhood[2] = pixels[(i - 1) * IMG_WIDTH + j];  
            neighbourhood[3] = pixels[(i - 1) * IMG_WIDTH + j - 1];
            neighbourhood[4] = pixels[i * IMG_WIDTH + j - 1];  
            neighbourhood[5] = pixels[(i + 1) * IMG_WIDTH + j - 1];
            neighbourhood[6] = pixels[(i + 1) * IMG_WIDTH + j];  
            neighbourhood[7] = pixels[(i + 1) * IMG_WIDTH + j + 1];

            transform = 0;
            loop_neighbourhood: for(kk = 0; kk < 8; kk++){
                if(pixels[i * IMG_WIDTH + j] < neighbourhood[kk])
                    transform = transform  + (1 << kk);

            }
            histogram[transform]++;

        }
    }

    // Calcular la suma de los cuadrados de los elementos
    uint32_t sum_of_squares = 0.0;
    for (int i = 0; i < HISTOGRAM_WIDTH; i++) {
        sum_of_squares += histogram[i] * histogram[i];
    }
    
    // Normalizar el vector
    double norm = sqrt(sum_of_squares); // Esto sería la norma L2, pero no la calculamos



    // Normalizar el vector
    loop_division: for (int i = 0; i < HISTOGRAM_WIDTH; i++) {
        feature[i] = histogram[i] / norm;
    }
 
}
#endif


#ifdef IMPROVEMENT
void extract_histogram(uint8_t pixels[IMG_HEIGHT * IMG_WIDTH / SPLIT + 2 * IMG_WIDTH], uint16_t histogram[HISTOGRAM_WIDTH], uint8_t end){

    uint8_t neighbourhood[8];
    uint8_t transform;
    int i, j, kk;



    loop_height: for (i = 1; i < IMG_HEIGHT /SPLIT + end - 1; i++){
        loop_with: for (j = 1; j < IMG_WIDTH - 1; j++){

            neighbourhood[0] = pixels[i * IMG_WIDTH + j + 1];
            neighbourhood[1] = pixels[(i - 1) * IMG_WIDTH + j + 1];
            neighbourhood[2] = pixels[(i - 1) * IMG_WIDTH + j];
            neighbourhood[3] = pixels[(i - 1) * IMG_WIDTH + j - 1];
            neighbourhood[4] = pixels[i * IMG_WIDTH + j - 1];
            neighbourhood[5] = pixels[(i + 1) * IMG_WIDTH + j - 1];
            neighbourhood[6] = pixels[(i + 1) * IMG_WIDTH + j];
            neighbourhood[7] = pixels[(i + 1) * IMG_WIDTH + j + 1];

            transform = 0;
            loop_neighbourhood: for(kk = 0; kk < 8; kk++){
                if(pixels[i * IMG_WIDTH + j] < neighbourhood[kk])
                    transform = transform  + (1 << kk);

            }
            histogram[transform]++;

        }
    }
}

void LBPFeatures(uint8_t pixels[IMG_HEIGHT*IMG_WIDTH], float feature[HISTOGRAM_WIDTH]){

    int i;
    uint16_t histogram[HISTOGRAM_WIDTH] = {0};
    uint32_t histogram_product[HISTOGRAM_WIDTH] = {0};

    loop_split: for (i = 0; i < SPLIT; i++) {
        if(i == SPLIT - 1){
            extract_histogram(pixels + i * IMG_HEIGHT * IMG_WIDTH / SPLIT, histogram , 0);
        }else{
            extract_histogram(pixels + i * IMG_HEIGHT * IMG_WIDTH / SPLIT, histogram , 2);
        }
    }

    loop_product: for (i = 0; i < HISTOGRAM_WIDTH; i++) {
        histogram_product[i] = histogram[i] * histogram[i];
    }
    uint16_t dim = HISTOGRAM_WIDTH;

    loop_acum_while: while (dim > 1) {
    	loop_acum_for: for (i = 0; i < dim / 2; i++) {
            histogram_product[i] += histogram_product[i + dim / 2];
        }
        dim /= 2;
    }

    double norm = sqrt(histogram_product[0]);

    loop_division: for (i = 0; i < HISTOGRAM_WIDTH; i++) {
        feature[i] = histogram[i] / norm;
    }

}
#endif

#ifdef IMPROVEMENT2

float sqrt_hls(float number) {

    union {
        float f;
        uint32_t i;
    } conv = { number };

    conv.i = 0x1fbd1df5 + (conv.i >> 1);
    conv.f = 0.5f * (conv.f + number / conv.f);

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

    float norm = sqrt_hls(acum);

    loop_division: for (i = 0; i < HISTOGRAM_WIDTH; i++) {
        feature[i] = histogram[0][i] / norm;
    }

}

#endif

#ifdef IMPROVEMENT3

#define MAX_ITERATIONS 19

float sqrt_hls(float num) {

    float low = 0, high = num, mid, ans;
    uint16_t i;

    loop_sqrt: for (i = 0; i < MAX_ITERATIONS; i++) {
        mid = 0.5 * (low + high);
        (mid * mid < num) ? (low = mid) : (high = mid);
    }

    ans = (low + high) * 0.5;
    return ans;
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

    uint16_t i,j;

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

    loop_split2: for (j = 0; j < SPLIT; j++) {
        uint16_t current_slice_size = (j == SPLIT - 1) ? slice_size : extended_slice_size;
        loop_split1: for (i = 0; i < current_slice_size; i++) {
            slice_pixels[j][i] = pixels[i + j * slice_size];
        }
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

    float norm = sqrt_hls(acum);


    loop_division: for (i = 0; i < HISTOGRAM_WIDTH; i++) {
        feature[i] = histogram[0][i] / norm;
    }

}

#endif
