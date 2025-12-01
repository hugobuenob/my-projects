#include "Predict_SVM_SC.h"
#include "LBPFeatures_SC.h"
#include <systemc.h>

void Classifier::histogram_generation(){

  loop_height: for (i = 1; i < IMG_HEIGHT - 1; i++){
    loop_width: for (j = 1; j < IMG_WIDTH - 1; j++){

          //Neighbourhood =[I(i,j+1) I(i-1,j+1) I(i-1,j) I(i-1,j-1) I(i,j-1) I(i+1,j-1) I(i+1,j) I(i+1,j+1)];
          neighbourhood[0] = pixels[i * IMG_WIDTH + j + 1].read();
          neighbourhood[1] = pixels[(i - 1) * IMG_WIDTH + j + 1].read();
          neighbourhood[2] = pixels[(i - 1) * IMG_WIDTH + j].read();
          neighbourhood[3] = pixels[(i - 1) * IMG_WIDTH + j - 1].read();
          neighbourhood[4] = pixels[i * IMG_WIDTH + j - 1].read();
          neighbourhood[5] = pixels[(i + 1) * IMG_WIDTH + j - 1].read();
          neighbourhood[6] = pixels[(i + 1) * IMG_WIDTH + j].read();
          neighbourhood[7] = pixels[(i + 1) * IMG_WIDTH + j + 1].read();

          transform = 0;
          loop_neighbourhood: for(kk = 0; kk < 8; kk++){
              if(pixels[i * IMG_WIDTH + j] < neighbourhood[kk])
                  transform = transform  + (1 << kk); //transform.write(transform.read()+1)
          }
          histogram[transform]++; // histogram[i].write(histogram[i].read() + 1);
    }
  }

  // Calcular la suma de los cuadrados de los elementos
  sum_of_squares = 0.0;
  for (i = 0; i < HISTOGRAM_WIDTH; i++) {
      sum_of_squares += histogram[i] * histogram[i];
  }

  // Normalizar el vector
  norm = sqrt(sum_of_squares); // Esto ser�a la norma L2, pero no la calculamos

  loop_division: for (int i = 0; i < HISTOGRAM_WIDTH; i++) {
    //features[i].write(histogram[i]/ norm);		//.write() o no hace falta?
    features[i] = histogram[i]/ norm;
  }
}

void Classifier::classification(){

  biggest_distance = 0;

  //Cover all the individual binary classifiers (for each of the 24)
  for (k = 0; k<BINARY_CLASSIFIERS_WIDTH; k++) {

      class_distance = Bias.read();

      //For all the features in the input set (256)
      for (i = 0; i<HISTOGRAM_WIDTH; i++) {
          //predictors[i] = (features[i].read() - Mu[i].read())/Sigma[i].read();    //Vector 1x256
	  predictors[i] = (features[i] - Mu[i].read())/Sigma[i].read();    //Vector 1x256


      //SVM Parameters.                                                           //Vector 1x256
      z[i] = predictors[i]/Scale.read();                                          //Vector 1x256
      w[i] = Beta[i].read();                                                      //Vector 256x1
      class_distance += z[i]*w[i];
      }

      //Store the distance to the Hyperplane corresponding to each class.
      if (biggest_distance < class_distance)
          biggest_distance = class_distance;          //Devuelve el �ndice de la clase asignada
          index = k;
  }
  Prediction.write(index);
}
