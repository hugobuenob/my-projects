#include <systemc.h>
#include "counter.h"
#include "counter_tb.h"
#include "top.h"

int sc_main(int argc, char* argv[]){

    sc_signal<bool> clk_tb, count_tb, reset_tb;
    sc_signal<int> q_tb;

    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
    
    //Time resolution of the simulation
    sc_set_time_resolution(1, SC_NS);

    counter *counter_inst;
    counter_inst = new counter("counter");
    counter_inst -> reset(reset_tb);
    counter_inst -> clk(clk_tb);
    counter_inst -> count(count_tb);
    counter_inst -> q(q_tb);

    counter_tb *counter_tb_inst;
    counter_tb_inst = new counter_tb("counter_tb");
    counter_tb_inst -> reset(reset_tb);
    counter_tb_inst -> clk(clk_tb);
    counter_tb_inst -> count(count_tb);
    //counter_tb_inst -> q(q_tb); 


    //Create a trace file.
    sc_trace_file* tf = sc_create_vcd_trace_file("trace_NAME");
    sc_trace(tf, reset_tb, "reset");
    sc_trace(tf, clk_tb, "clk");
    sc_trace(tf, count_tb, "count");
    sc_trace(tf, q_tb, "q");

    //Start Simulation
	sc_start(500, SC_NS);
	cout << " Finish Simulation \n";
	sc_close_vcd_trace_file(tf);

    return(0);

}
