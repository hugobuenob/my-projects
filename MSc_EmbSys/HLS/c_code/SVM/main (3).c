#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "features_matrix.h"
#include "SVM.h"
#include "model.h"
#include "results.h"


int main() {

    uint8_t prediction;
    uint8_t error = 0;

    for(int i = 0; i < 4319; i++){
        SVM(features[i], mu, sigma, bias, scale, beta, &prediction);

        if(prediction != matlab_results[i]){
            error++;
        }
    }

    printf("Percentage error %i%%", error/4319.0);    

    return 0;
}
 