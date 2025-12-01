#ifndef SVM_SC_H
#define SVM_SC_H
#include <systemc.h>

#include <math.h>
#define BINARY_CLASSIFIERS_WIDTH 24
#define HISTOGRAM_WIDTH 256

#define ONE_PROCESS
//#define MANY_PROCESSES

SC_MODULE(SVM){
	int i, k; //i for the 256 components in the vectors of each Binary Classifier, k for the 24 Binary Classifiers
  double predictors[HISTOGRAM_WIDTH-1];  //length(Infer_Features) = 256 = HISTOGRAM_WIDTH
	double z[HISTOGRAM_WIDTH], w[HISTOGRAM_WIDTH];
	double class_distance, biggest_distance;
	int index;

	public:
		sc_in<double> features[HISTOGRAM_WIDTH];
		sc_in<double> Bias;
		sc_in<double> Beta[HISTOGRAM_WIDTH];
		sc_in<double> Mu[HISTOGRAM_WIDTH];
		sc_in<double> Scale;
		sc_in<double> Sigma[HISTOGRAM_WIDTH];
		sc_out<int> Prediction;

		void classification();

	SC_CTOR(SVM){
		SC_METHOD(classification);
		sensitive << features << Bias << Beta << Mu << Scale << Sigma;    // ¿Hay que poner los corchetes?
	}
};

#endif
