#include "counter.h"
#include "counter_tb.h"
#include <systemc.h>

#ifndef TOP_H
#define TOP_H

class top: public sc_module {
	public:
		sc_signal<bool> clk_sig, count_sig, reset_sig;
		sc_signal<int> q_sig;
		counter uut; 	//Counter instance
		counter_tb tb; 	//TB instance
		
	top(sc_module_name nm): sc_module(nm), uut("uut"), tb("tb") {
		
		tb.clk(clk_sig);
		tb.reset(reset_sig);
		tb.count(count_sig);
		uut.clk(clk_sig);
		uut.reset(reset_sig);
		uut.count(count_sig);
		uut.q(q_sig);
	}
};

#endif
