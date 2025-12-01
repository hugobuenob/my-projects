#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "expected_histogram.h"
#include "LBPFeatures_custom.h"

#define EPSILON 0.000001

uint8_t *readBinaryFile(const char *fileName) {
    FILE *file = fopen(fileName, "rb");
    if (file == NULL) {
        printf("Unable to open the file.\n");
        return NULL;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    // Read the entire content of the file
    uint8_t *buffer = (uint8_t *)malloc(fileSize * sizeof(uint8_t));
    if (buffer == NULL) {
        printf("Error allocating memory.\n");
        fclose(file);
        return NULL;
    }

    size_t result = fread(buffer, sizeof(uint8_t), fileSize, file);
    if (result != fileSize) {
        printf("Error reading the file.\n");
        fclose(file);
        free(buffer);
        return NULL;
    }

    // Free memory and close the file
    return buffer;
    fclose(file);
}

uint32_t test_results(float *features_hardware){

    uint32_t error = 0;
    uint32_t i = 0;

    for (i = 0; i < HISTOGRAM_WIDTH; i++){
        if(fabs(features_hardware[i] - features[i]) > EPSILON){
            error = 1;
            printf("features_hardware[%u] = %f != features[%u] = %f\n",i , features_hardware[i], i, features[i]);
        }
    }

    return error;
}

int main() {

    const char *filename = "raw_image.bin";
    uint8_t *pixels = readBinaryFile(filename);
    float features_hardware[HISTOGRAM_WIDTH];
    uint32_t i = 0;

    LBPFeatures_custom(pixels, features_hardware);

    for (i = 0; i < HISTOGRAM_WIDTH; i++){
        printf("histogram[%u] = %f \n", i, features_hardware[i]);
    }

    if (test_results(features_hardware)){
        printf("ERROR in implementation\n");
        free(pixels);
        return 1;

    } else {
        printf("Implementation OK\n");
        free(pixels);
        return 0;
    }
}
