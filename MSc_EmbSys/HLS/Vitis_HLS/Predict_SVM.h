#ifndef SVM_H
#define SVM_H

#include <math.h>
#define BINARY_CLASSIFIERS_WIDTH 24
#define HISTOGRAM_WIDTH 256

//#define WITHOUT_STRUCT
//#define WITH_STRUCT
#define WITH_STRUCT_2


//uint8_t Predict_SVM (float Infer_Features[], float Classes_Distances[BINARY_CLASSIFIERS_WIDTH], float *Beta, float *Bias, float *Scale, float *Mu, float *Sigma); //WITHOUT_STRUCT
//uint8_t Predict_SVM (float Infer_Features[], float Classes_Distances[BINARY_CLASSIFIERS_WIDTH], struct binarylearner svmlinear[BINARY_CLASSIFIERS_WIDTH]);          //WITH_STRUCT
void Predict_SVM (float Infer_Features[], float Classes_Distances[BINARY_CLASSIFIERS_WIDTH], struct binarylearner svmlinear[BINARY_CLASSIFIERS_WIDTH], uint8_t *Prediction);          //WITH_STRUCT_2

#endif
