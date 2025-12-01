#include <systemc.h>
#include "fsm.h"
#include "fsm_tb.h"

int sc_main(int argc, char* argv[]){

    sc_signal<bool> clk_tb, a_tb, b_tb, reset_tb;
    sc_signal<int> open_tb;

    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
    
    //Time resolution of the simulation
    sc_set_time_resolution(1, SC_NS);

    fsm *fsm_inst;
    fsm_inst = new fsm("fsm");
    fsm_inst -> reset(reset_tb);
    fsm_inst -> clk(clk_tb);
    fsm_inst -> a(a_tb);
    fsm_inst -> b(b_tb);
    fsm_inst -> open(open_tb);

    fsm_tb *fsm_tb_inst;
    fsm_tb_inst = new fsm_tb("fsm_tb");
    fsm_tb_inst -> reset(reset_tb);
    fsm_tb_inst -> clk(clk_tb);
    fsm_tb_inst -> a(a_tb);
    fsm_tb_inst -> b(b_tb);
    fsm_tb_inst -> open(open_tb);


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
