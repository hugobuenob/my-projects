function [Features] = LBPFeatures_custom(I)

format long;
int Transform[128][128] = 0;

//Iterate over every pixel in the 128x128 image
for (i=2; i<128; i++) {
  for (j=2; j<128; j++) {

  //Extract the neighbourhood of the pixel.
  int Neighbourhood[9] = {I(i,j+1), I(i-1,j+1), I(i-1,j), I(i-1,j-1), I(i,j-1), I(i+1,j-1), I(i+1,j), I(i+1,j+1)};

   //Transform the neighbourhood to binary values.
   for () kk = 1:1:8 {
      if (I(i,j) < Neighbourhood(kk)) {
        Transform(i,j) = Transform(i,j) + 2ˆ(kk-1);
      }
    }
  }
}

//Create the histogram
/* int */ I1_Features_hist = histcounts( reshape(Transform(2:127,2:127), [1,126*126]), 256);

//Normalize the histogram.
Features = I1_Features_hist/vecnorm(I1_Features_hist,2);

//Features to compare
//I1_Features = extractLBPFeatures(I,'Upright',false,'NumNeighbors',8,'Radius',1);
//bar([I1_Features_hist ; I1_Features]','grouped')
