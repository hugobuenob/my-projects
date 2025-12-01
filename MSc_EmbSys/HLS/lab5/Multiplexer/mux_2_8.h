#include <systemc.h>
#ifndef MUX_H
#define MUX_H

SC_MODULE(Mux){
	public:
		sc_in<bool> in0, in1, in2,in3;
		sc_in<unsigned int> sel;
		sc_out<bool> out0;

		void Mux_behaviour();

	SC_CTOR(Mux){
		SC_METHOD(Mux_behaviour);
		sensitive << in0 << in1 << in2 << in3 << sel;


	}

	
};

#endif