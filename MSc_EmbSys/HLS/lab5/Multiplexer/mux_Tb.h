#ifndef MUX_TB_H
#define MUX_TB_H
#include <systemc.h>

SC_MODULE(Mux_Tb) {
    public:
        sc_out<bool> in0, in1, in2, in3;
        sc_out<unsigned int> sel;

        void Mux_stimuli();

        SC_CTOR(Mux_Tb){
            SC_THREAD(Mux_stimuli);
        }
};

#endif