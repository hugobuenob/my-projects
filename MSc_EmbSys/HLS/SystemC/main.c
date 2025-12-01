#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "classifier.h"
#include "results.h"
#include "model.h"

#define MAX_TEST_IMAG 4319
#define NUM_TEST_IMGA MAX_TEST_IMAG
#define EPSILON 0.005

uint8_t readBinaryFile(const char *fileName, uint8_t *buffer) {
    FILE *file = fopen(fileName, "rb");
    if (file == NULL) {
        printf("Unable to open the file. \n");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);


    size_t result = fread(buffer, sizeof(char), fileSize, file);
    if (result != fileSize) {
        printf("Error reading the file.\n");
        fclose(file);
        exit(1);
    }

    fclose(file);
}

int main() {

    char filename[300];  
    uint8_t prediction;
    uint16_t error = 0;
    uint32_t Nimg = 0;
    uint8_t pixels[IMG_WIDTH*IMG_HEIGHT];
    
    for(Nimg = 0; Nimg < NUM_TEST_IMGA; Nimg++){

        sprintf(filename, "../rawImg/%06d.raw", Nimg);
        readBinaryFile(filename, pixels);

        classifier(pixels, mu, sigma, bias, scale, beta, &prediction);

        if(prediction != matlab_results[Nimg]){
            error++;
            printf("Error image %i\n", Nimg);
        }
    }

    printf("Percentage error %f%%\n", (error/(float)NUM_TEST_IMGA)); 

    return 0;
}
