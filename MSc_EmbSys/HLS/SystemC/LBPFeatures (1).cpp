#include "LBPFeatures_SC.h"
#include <systemc.h>

#ifdef ONE_PROCESS
void LBPFeatures::histogram_generation(){

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

          /* Incorrect. The read & write are just for interfaces between modules, not within the same module. Right???
          neighbourhood[0].write(pixels[i * IMG_WIDTH + j + 1].read());
          neighbourhood[1].write(pixels[(i - 1) * IMG_WIDTH + j + 1].read());
          neighbourhood[2].write(pixels[(i - 1) * IMG_WIDTH + j].read());
          neighbourhood[3].write(pixels[(i - 1) * IMG_WIDTH + j - 1].read());
          neighbourhood[4].write(pixels[i * IMG_WIDTH + j - 1].read());
          neighbourhood[5].write(pixels[(i + 1) * IMG_WIDTH + j - 1].read());
          neighbourhood[6].write(pixels[(i + 1) * IMG_WIDTH + j].read());
          neighbourhood[7].write(pixels[(i + 1) * IMG_WIDTH + j + 1].read());
          */

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
    features[i].write(histogram[i]/ norm);
  }
}

#endif

#ifdef MANY_PROCESSES

void LBPFeatures::transform(){

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

          /* Incorrect. The read & write are just for interfaces between modules, not within the same module. Right???
          neighbourhood[0].write(pixels[i * IMG_WIDTH + j + 1].read());
          neighbourhood[1].write(pixels[(i - 1) * IMG_WIDTH + j + 1].read());
          neighbourhood[2].write(pixels[(i - 1) * IMG_WIDTH + j].read());
          neighbourhood[3].write(pixels[(i - 1) * IMG_WIDTH + j - 1].read());
          neighbourhood[4].write(pixels[i * IMG_WIDTH + j - 1].read());
          neighbourhood[5].write(pixels[(i + 1) * IMG_WIDTH + j - 1].read());
          neighbourhood[6].write(pixels[(i + 1) * IMG_WIDTH + j].read());
          neighbourhood[7].write(pixels[(i + 1) * IMG_WIDTH + j + 1].read());
          */

          transform = 0;
          loop_neighbourhood: for(kk = 0; kk < 8; kk++){
              if(pixels[i * IMG_WIDTH + j] < neighbourhood[kk])
                  transform = transform  + (1 << kk); //transform.write(transform.read()+1)
          }
          histogram[transform]++; // histogram[i].write(histogram[i].read() + 1);
    }
  }
}

void sum_of_squares(){
  // Calcular la suma de los cuadrados de los elementos + Normalizar el vector
  for (int i = 0; i < HISTOGRAM_WIDTH; i++) {
      norm.write(sqrt(histogram[i].read() * histogram[i].read()));
  }
}

void LBPFeatures::histogram(){

    // Normalizar el vector
    loop_division: for (int i = 0; i < HISTOGRAM_WIDTH; i++) {
        features[i].write(histogram[i].read() / norm.read());
    }
}

#endif
