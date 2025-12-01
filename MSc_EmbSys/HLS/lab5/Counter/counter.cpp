#include <systemc.h>
#include "counter.h"

void counter::do_count(){

	if(reset.read() == 1){
			value = 0;
	}
	else if (count.read() ==1 & clk.read() == 1 ){
		value += 1;
	}
	q.write(value);
}
