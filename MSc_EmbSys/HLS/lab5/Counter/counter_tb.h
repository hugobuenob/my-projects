#include <systemc.h>

#ifndef COUNTER_TB_H
#define COUNTER_TB_H

SC_MODULE(counter_tb) {
    public:
        sc_out<bool> clk, reset, count;
		
        void clk_gen();
        void stimuli();
        
        SC_CTOR(counter_tb){
            SC_THREAD(clk_gen);
            SC_THREAD(stimuli);
        }
};

#endif
