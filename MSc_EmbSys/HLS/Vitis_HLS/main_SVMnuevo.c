#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include "expected_histogram.h"
//#include "LBPFeatures_custom.h"

#define EPSILON 0.000001

//The struct is defined and later completed with the data read from the .bin files
struct binarylearner {
  float Beta[]    //Vector 256x1
  float Bias      //Escalar 1x1
  float Scale     //Escalar 1x1
  float Mu[]      //Vector 1x256
  float Sigma[]   //Vector 1x256
}

uint8_t *readBinaryFile(const char *fileName) {   //Alguien (Andrés, Rodrigo o Álvaro) ha sugerido que lo meta en Predict_SVM.c
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

uint32_t test_results(float *Classes_Distances){

    uint32_t error = 0;
    uint32_t i = 0;

    for (i = 0; i < HISTOGRAM_WIDTH; i++){
        if(fabs(Classes_Distances[i] - distances[i]) > EPSILON){                //DE DÓNDE SACA FEATURES[I]? De expected_histogram.h ??
            error = 1;
            printf("features_hardware[%u] = %f != features[%u] = %f\n",i , Classes_Distances[i], i, distances[i]);
        }
    }
    return error;
}

int main() {

    uint32_t i, k = 0;
    float Classes_Distances[BINARY_CLASSIFIERS_WIDTH];

    //Por cada binary learner...
    const char *Beta_name  = "Beta.bin";  //Vector 256x1
    const char *Bias_name  = "Bias.bin";  //Escalar 1x1
    const char *Scale_name = "Scale.bin"; //Escalar 1x1
    const char *Mu_name    = "Mu.bin";    //Vector 1x256
    const char *Sigma_name = "Sigma.bin"; //Vector 1x256
    //const char *SVM_Model_name = "SVM_Model.bin"                              //Esto sería si guardara el struct tal cual desde Matlab

    //Parameters to be passed to the Predict_SVM function
    float *Beta_read  = readBinaryFile(Beta_name);
    float *Bias_read  = readBinaryFile(Bias_name);
    float *Scale_read = readBinaryFile(Scale_name);
    float *Mu_read    = readBinaryFile(Mu_name);
    float *Sigma_read = readBinaryFile(Sigma_name);

    struct binarylearner SVM_Model[BINARY_CLASSIFIERS_WIDTH];
    //struct binarylearner SVM_Model[] = readBinaryFile(SVM_Model_name);        //Esto sería si guardara el struct tal cual desde Matlab

    //Initialise the struct SVM_Model (rejoin what was disjoint in Matlab)
    for (k=0; k<HISTOGRAM_WIDTH, k++) {
      SVM_Model(k).Bias   = Bias_read;
      SVM_Model(k).Scale  = Scale_read;
      for (i=0, i<BINARY_CLASSIFIERS_WIDTH, i++)  //Cuidado! Beta está traspuesto con respecto a Mu y Sigma
        SVM_Model(k).Beta(i)  = Beta_read(i);
        SVM_Model(k).Mu(i)    = Mu_read(i);
        SVM_Model(k).Sigma(i) = Sigma_read(i);
      }
    }

    /*  No he creado el archivo con los resultados de Matlab porque no me funciona

    //Read Matlab test_results
    const char *filename = "raw_image2.bin";
    uint8_t *distances = readBinaryFile(filename);

    */

    //Invoke Predict_SVM function
    //Predict_SVM (Infer_Features, Classes_Distances, Beta_read, Bias_read, Scale_read, Mu_read, Sigma_read);
    Predict_SVM (Infer_Features, Classes_Distances, SVM_Model);

    /*  No he creado el archivo con los resultados de Matlab porque no me funciona

    if (test_results(Classes_Distances)) {
        printf("ERROR in implementation\n");
        free(pixels);
        return 1;

    } else {
        printf("Implementation OK\n");
        free(pixels);
        return 0;
    }

    */

    return 0;
}
