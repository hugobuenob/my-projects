#ifndef FSM_TB_H
#define FSM_TB_H

#include <systemc.h>


SC_MODULE(fsm_tb) {
    public:
        sc_out<bool> a, b, clk, reset;
		
        void clk_gen();
        void stimuli();
        
        SC_CTOR(fsm_tb){
            SC_THREAD(clk_gen);
            SC_THREAD(stimuli);
        }
};

#endif
