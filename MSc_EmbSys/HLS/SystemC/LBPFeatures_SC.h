#ifndef LBPFeatures_H
#define LBPFeatures_H
#include <systemc.h>

#define IMG_WIDTH 128
#define IMG_HEIGHT 128
#define HISTOGRAM_WIDTH 256
//#define SPLIT 32

#define ONE_PROCESS
//#define MANY_PROCESSES

SC_MODULE(LBPFeatures){
	int neighbourhood[8];
	int transform;
	int i, j, kk;
	int histogram[HISTOGRAM_WIDTH];

































	int sum_of_squares;
	double norm;

	public:
		sc_in<int> pixels[IMG_HEIGHT*IMG_WIDTH];
		sc_out<double> features[HISTOGRAM_WIDTH];

		void histogram_generation();

		//¿Dividir en varias funciones?
		/*
		void neighbourhood();
		void sum_of_squares();
		void histogram();
		*/

	SC_CTOR(LBPFeatures){
		SC_METHOD(histogram_generation);
		sensitive << pixels;    // ¿Hay que poner los corchetes?
	}
};

#endif
