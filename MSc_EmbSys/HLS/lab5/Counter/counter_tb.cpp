#include "counter_tb.h"
#include <systemc.h>

void counter_tb::clk_gen() {
	while(true) {
		clk.write(true);
		wait(10,SC_NS);
		clk.write(false);
		wait(10,SC_NS);
	}
}


void counter_tb::stimuli() {
	while(true) {
	reset.write(true);
	count.write(false);
	wait(10,SC_NS);
	count.write(true);
	wait(10,SC_NS);
	count.write(false);
	wait(10,SC_NS);
	count.write(true);
	wait(10,SC_NS);
	count.write(false);
	wait(10,SC_NS);
	count.write(true);
	wait(10,SC_NS);
	count.write(false);
	wait(10,SC_NS);
	count.write(true);
	wait(10,SC_NS);
	count.write(false);
	wait(10,SC_NS);
	count.write(true);
	wait(10,SC_NS);
	}
}
