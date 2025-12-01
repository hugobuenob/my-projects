#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "LBPFeatures.h"
#include "features_matrix.h"


#define EPSILON 0.005

void readBinaryFile(const char *fileName, uint8_t *buffer) {
    FILE *file = fopen(fileName, "rb");
    if (file == NULL) {
        printf("Unable to open the file. \n");
        exit(-1);
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);


    size_t result = fread(buffer, sizeof(char), fileSize, file);
    if (result != fileSize) {
        printf("Error reading the file.\n");
        fclose(file);
        exit(-1);
    }

    fclose(file);
}

uint32_t test_results(float *features_hardware, uint32_t iteration){

    uint32_t i = 0;

    for (i = 0; i < HISTOGRAM_WIDTH; i++){
        if(fabs(features_hardware[i] - features[iteration][i])/features[iteration][i] > EPSILON){
            return 1;
            //printf("features_hardware[%u] = %f != features[%u] = %f\n",i , features_hardware[i], i, features[i]);
        }
    }
    
    return 0;
}

int main() {

    char filename[300];  
    uint32_t erro_num=0;  
    float features_hardware[HISTOGRAM_WIDTH];
    uint32_t i, Nimg = 0;
    uint8_t pixels[IMG_WIDTH*IMG_HEIGHT];
    for(Nimg = 0; Nimg < 1; Nimg++){

        sprintf(filename, "rawImg/%06d.raw", Nimg);
         readBinaryFile(filename, pixels);

        LBPFeatures(pixels, features_hardware);

        if (test_results(features_hardware, Nimg)){
            printf("ERROR in implementation image %i, %f\n", Nimg, features_hardware[0]);
            erro_num++;

        }else{
            printf("Implementation OK image %i, %f\n", Nimg, features_hardware[0]);
            
        }
    }

    printf("NUmber of errors %i of %i \n", erro_num, Nimg);

    return 0;
}
