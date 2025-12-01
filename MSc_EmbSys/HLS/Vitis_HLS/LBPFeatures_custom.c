#include "LBPFeatures_custom.h"

#ifdef STANDAR

void LBPFeatures_custom(uint8_t pixels[IMG_HEIGHT*IMG_WIDTH], float feature[HISTOGRAM_WIDTH]){

    int i, j, kk;
    uint8_t neighbourhood[8];
    uint8_t transform;
    uint16_t histogram[HISTOGRAM_WIDTH] = {0};

    loop_height: for (i = 1; i < IMG_HEIGHT - 1; i++){
      loop_width: for (j = 1; j < IMG_WIDTH - 1; j++){


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

            //Transform the 8-bit vector to a decimal value between 0 and 255
            loop_neighbourhood: for(kk = 0; kk < 8; kk++) {
                                  if(pixels[i * IMG_WIDTH + j] < neighbourhood[kk])
                                      transform = transform  + (1 << kk);
                                } // 1<<kk qué significa? Y no hay que poner transform a 0 cada vez?
            histogram[transform]++;
        }
    }

    // Calcular la suma de los cuadrados de los elementos
    uint32_t sum_of_squares = 0.0;
    for (int i = 0; i < HISTOGRAM_WIDTH; i++) {
        sum_of_squares += histogram[i] * histogram[i];
    }

    // Calcular norma
    double norm = sqrt(sum_of_squares); // Esto seria la norma L2, pero no la calculamos

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
            loop_neighbourhood: for(kk = 0; kk < 8; kk++) {
                if(pixels[i * IMG_WIDTH + j] < neighbourhood[kk])
                    transform = transform  + (1 << kk);
            }
            histogram[transform]++;
        }
    }
}

void LBPFeatures_custom(uint8_t pixels[IMG_HEIGHT*IMG_WIDTH], float feature[HISTOGRAM_WIDTH]){

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

float sqrt_newton_raphson(float number) {
    float guess = number / 2.0;
    for (int i = 0; i < 20; i++) {
        guess = (guess + number / guess) / 2.0;
    }
    return guess;
}

void extract_histogram(uint8_t pixels[IMG_HEIGHT * IMG_WIDTH / SPLIT + 2 * IMG_WIDTH], uint16_t histogram[HISTOGRAM_WIDTH], uint8_t length){

    uint8_t neighbourhood[8];
    uint8_t transform;
    int i, j, kk;

    loop_height: for (i = 1; i < length; i++){
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

void LBPFeatures_custom(uint8_t pixels[IMG_HEIGHT*IMG_WIDTH], float feature[HISTOGRAM_WIDTH]){

    int i,j;

    uint16_t histogram[SPLIT][HISTOGRAM_WIDTH] = {0};
    uint8_t slice_pixels[SPLIT][IMG_HEIGHT*IMG_WIDTH/SPLIT+2*IMG_WIDTH] = {0};
    uint32_t histogram_product[HISTOGRAM_WIDTH] = {0};

    for (j = 0; j < SPLIT; j++) {
        if (j == SPLIT - 1) {
            for (i = 0; i < IMG_HEIGHT * IMG_WIDTH / SPLIT + 1 * IMG_WIDTH; i++) {
                slice_pixels[j][i] = pixels[i + j * IMG_HEIGHT * IMG_WIDTH / SPLIT - 1 * IMG_WIDTH];
            }
        } else if (j == 0) {
            for (i = 0; i < IMG_HEIGHT * IMG_WIDTH / SPLIT + 1 * IMG_WIDTH; i++) {
                slice_pixels[j][i] = pixels[i + j * IMG_HEIGHT * IMG_WIDTH / SPLIT ];
            }
        } else {
            for (i = 0; i < IMG_HEIGHT * IMG_WIDTH / SPLIT  + 2 * IMG_WIDTH; i++) {
                slice_pixels[j][i] = pixels[i + j * IMG_HEIGHT * IMG_WIDTH / SPLIT - 1 * IMG_WIDTH];
            }
        }
    }

    for (i = 0; i < SPLIT; i++) {
        if(i > 0 && i < SPLIT - 1)
            extract_histogram(slice_pixels[i], histogram[i], IMG_HEIGHT/SPLIT + 1);
        else
            extract_histogram(slice_pixels[i], histogram[i], IMG_HEIGHT/SPLIT + 0);
    }

    for (j = 1; j < SPLIT; j++) {
        for (i = 0; i < HISTOGRAM_WIDTH; i++) {

            histogram[0][i] += histogram[j][i];
        }
    }

    for (i = 0; i < HISTOGRAM_WIDTH; i++) {
        histogram_product[i] = histogram[0][i] * histogram[0][i];
    }
    uint16_t dim = HISTOGRAM_WIDTH;

    while (dim > 1) {
        for (i = 0; i < dim / 2; i++) {
            histogram_product[i] += histogram_product[i + dim / 2];
        }
        dim /= 2;
    }

    double norm = sqrt_newton_raphson(histogram_product[0]);

    loop_division: for (i = 0; i < HISTOGRAM_WIDTH; i++) {
        feature[i] = histogram[0][i] / norm;
    }

}

#endif
