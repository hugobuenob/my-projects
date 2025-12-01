#include "Predict_SVM_SC.h"
#include <systemc.h>

void SVM::classification(){

  biggest_distance = 0;

  //Cover all the individual binary classifiers (for each of the 24)
  for (k = 0; k<BINARY_CLASSIFIERS_WIDTH; k++) {

      class_distance = Bias.read();

      //For all the features in the input set (256)
      for (i = 0; i<HISTOGRAM_WIDTH; i++) {
          predictors(i) = (features(i).read() - Mu(i).read())/Sigma(i).read();    //Vector 1x256

      //SVM Parameters.                                                           //Vector 1x256
      z[i] = predictors(i)/Scale.read();                                          //Vector 1x256
      w[i] = Beta(i).read();                                                      //Vector 256x1
      class_distance += z(i)*w(i);
      }

      //Store the distance to the Hyperplane corresponding to each class.
      if (biggest_distance < class_distance)
          biggest_distance = class_distance;                                                   //Devuelve el �ndice de la clase asignada
          index = k;
  }
  Prediction.write(index);
}
