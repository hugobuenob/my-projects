/*
*     Mejoras implementadas o por implementar:
*
*   - Utilizar WITH_STRUCT parece más sensato
*   - Beta es un vector 256x1, mientras que Mu y Sigma son vectores 1x256. Es así porque luego se multiplican vectorialmente
*      para dar como resultado un escalar. Así, hay que multiplicarlos trasponiendo Beta (más complicado)
*   - Cambiar las condicionales de if() por las de ? - ahorra muchas LUTs y FFs
*   - Loop unrolling: quitar el loop y hacer las instrucciones una a una
*   - Declarar una variable en la función desde el principio para que no tenga que acceder constantemente a memoria a por ella
*   - Declarar TODO como double y luego como float nos permitiria ver una evolucion bestial frente a hacerlo directamente
*      solo en float. También en el archivo "ExportDataStrcutures"
*   - Hacer un solo vector largo (como en EPA) en lugar de matrices (pero la matriz es vector struct * vector de dentro)
*
*/

#include "Predict_SVM.h"

#ifdef WITHOUT_STRUCT   //Este está mal casi seguro. Mirar WITH_STRUCT mejor

uint8_t Predict_SVM (float Infer_Features[], float Classes_Distances[BINARY_CLASSIFIERS_WIDTH],
  float *Beta, float *Bias, float *Scale, float *Mu, float *Sigma) {

    //Store the distance to the binary classifier corresponding to each class.
    uint8_t Classes = [];
    uint8_t i, k; //i for the 256 components in the vectors of each Binary Classifier, k for the 24 Binary Classifiers
    float predictors[length(Infer_Features)-1];  //length(Infer_Features) = 256 = HISTOGRAM_WIDTH

    //Cover all the individual binary classifiers (for each of the 24)
    for (k = 0; k<BINARY_CLASSIFIERS_WIDTH; k++) {


        //For all the features in the input set (256)
        for (i = 0; i<length(Infer_Features); i++)
            predictors(i) = (Infer_Features(i) - Mu(i))/Sigma(i);

        //SVM Parameters.
        float b = Bias[i][j];                                                    //mejor vector "largo" que matriz?
        float z = Scale[i];
        float w = Beta[i][j];

        //Apply SVM Classifier.
        float predicted = z*w + b;

        //Store the distance to the Hyperplane corresponding to each class.
        for (i=0; i<BINARY_CLASSIFIERS_WIDTH; i++) {
          Classes_Distances[i] = predicted;
        }
    }

    Prediction = max(Classes_Distances);

    //The range of class labels starts in 0, the array index starts in 0.
    //Prediction = I-1;                                                          // No entiendo por qué -1

    return Prediction;
}

#endif

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

#ifdef WITH_STRUCT

struct binarylearner {
  float Beta[]    //Vector 256x1
  float Bias      //Escalar 1x1
  float Scale     //Escalar 1x1
  float Mu[]      //Vector 1x256
  float Sigma[]   //Vector 1x256
}

//Esta función devuelve un vector de 24 componentes binarios, sean 1 o -1, dependiendo de si "corresponden" a cada hiperplano.
//En realidad se decide con las distancias si hay varios que sí corresponden.
uint8_t Predict_SVM (float Infer_Features[], float Classes_Distances[BINARY_CLASSIFIERS_WIDTH], struct binarylearner svmlinear[BINARY_CLASSIFIERS_WIDTH]) {

    //Store the distance to the binary classifier corresponding to each class.
    //uint8_t Classes = [];   //Para qué sirve este?
    uint8_t i, k; //i for the 256 components in the vectors of each Binary Classifier, k for the 24 Binary Classifiers
    float predictors[length(Infer_Features)-1];  //length(Infer_Features) = 256 = HISTOGRAM_WIDTH

    //Cover all the individual binary classifiers (for each of the 24)
    for (k = 0; k<BINARY_CLASSIFIERS_WIDTH; k++) {

        //Select the binary classifier to be used (choose one of the 24, then repeat the loop)
        //svmlinear = SVM_Model.ClassificationSVM.BinaryLearners{k,1};

        //For all the features in the input set (256)
        for (i = 0; i<length(Infer_Features); i++) {
            predictors(i) = (Infer_Features(i) - svmlinear(k).Mu(i))/svmlinear(k).Sigma(i);     //Vector 1x256

        //SVM Parameters.                                         //Vector 1x256
        float z[length(Infer_Features)] = predictors(i)/svmlinear(k).Scale;                             //Vector 1x256
        float w[length(Infer_Features)] = svmlinear(k).Beta(i);                                         //Vector 256x1
        }

        float b = svmlinear(k).Bias;    //Esta variable podría reducir el tiempo de acceso a memoria por hacerlo una sola vez para todas las k? O no?

        //Apply SVM Classifier.
        //float predicted = z*w + b;                                              //Multiplicación vectorial. El resultado es un escalar. Más complicado.
        float predicted = b;
        for (i=0; i<; i++) {
          predicted += z(i)*w(i);
        }

        //Store the distance to the Hyperplane corresponding to each class.
        for (i=0; i<BINARY_CLASSIFIERS_WIDTH; i++) {
          Classes_Distances(i) = predicted;
        }
    }

    Prediction = max(Classes_Distances);

    //The range of class labels starts in 0, the array index starts in 0.
    //Prediction = I-1;

    return Prediction;
}

#endif

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

#ifdef WITH_STRUCT_2

struct binarylearner {
  float Beta[]    //Vector 256x1
  float Bias      //Escalar 1x1
  float Scale     //Escalar 1x1
  float Mu[]      //Vector 1x256
  float Sigma[]   //Vector 1x256
}

//Esta función devuelve un vector de 24 componentes binarios, sean 1 o -1, dependiendo de si "corresponden" a cada hiperplano.
//En realidad se decide con las distancias si hay varios que sí corresponden.
void Predict_SVM (float Infer_Features[HISTOGRAM_WIDTH], struct binarylearner svmlinear[BINARY_CLASSIFIERS_WIDTH], uint8_t *Prediction) {

    //Store the distance to the binary classifier corresponding to each class.
    //uint8_t Classes = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};   //Para qué sirve este?
    uint8_t i, k; //i for the 256 components in the vectors of each Binary Classifier, k for the 24 Binary Classifiers
    float predictors[length(Infer_Features)-1];  //length(Infer_Features) = 256 = HISTOGRAM_WIDTH

    //Cover all the individual binary classifiers (for each of the 24)
    for (k = 0; k<BINARY_CLASSIFIERS_WIDTH; k++) {

        //Select the binary classifier to be used (choose one of the 24, then repeat the loop)
        //svmlinear = SVM_Model.ClassificationSVM.BinaryLearners{k,1};

        //For all the features in the input set (256)
        for (i = 0; i<length(Infer_Features); i++) {
            predictors(i) = (Infer_Features(i) - svmlinear(k).Mu(i))/svmlinear(k).Sigma(i);     //Vector 1x256

        //SVM Parameters.                                         //Vector 1x256
        float z[length(Infer_Features)] = predictors(i)/svmlinear(k).Scale;                             //Vector 1x256
        float w[length(Infer_Features)] = svmlinear(k).Beta(i);                                         //Vector 256x1
        }

        float b = svmlinear(k).Bias;    //Esta variable podría reducir el tiempo de acceso a memoria por hacerlo una sola vez para todas las k? O no?

        //Apply SVM Classifier.
        //float predicted = z*w + b;                                              //Multiplicación vectorial. El resultado es un escalar. Más complicado.
        float class_distance = b;
        for (i=0; i<; i++) {
          class_distance += z(i)*w(i);
        }

        //Store the distance to the Hyperplane corresponding to each class.
        if (*Prediction < class_distance)
          *Prediction = k;                                                         //Devuelve el índice de la clase asignada
    }
}

#endif
