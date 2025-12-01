#include "fsm_tb.h"
#include <systemc.h>

void fsm_tb::clk_gen() {
	while(true) {
		clk.write(true);
		wait(10,SC_NS);
		clk.write(false);
		wait(10,SC_NS);
	}
}

void fsm_tb::stimuli() {
	while(true) {
	reset.write(true);
	a.write(false);
	b.write(false);
	wait(10,SC_NS);
	a.write(true);
	wait(10,SC_NS);
	a.write(false);
	b.write(true);
	wait(10,SC_NS);
	b.write(false);
	wait(10,SC_NS);
	a.write(true);
	b.write(true);
	wait(10,SC_NS);
	a.write(false);
	b.write(false);
	wait(10,SC_NS);
	}
}
