#include <systemc.h>

#ifndef COUNTER_H
#define COUNTER_H

SC_MODULE(counter)
{
	int value;
	public:
	sc_in<bool> clk;
	sc_in<bool> count;
	sc_in<bool> reset;
	sc_out<int> q;
	SC_HAS_PROCESS(counter);
	counter(sc_module_name nm): sc_module(nm), value(0) {
		SC_METHOD(do_count);
		sensitive << clk.pos() << reset;
	}
	void do_count();
};

#endif
