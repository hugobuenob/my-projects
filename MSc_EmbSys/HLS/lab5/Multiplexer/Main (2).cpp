#include <systemc.h>
#include "mux_Tb.h"
#include "mux_2_8.h"

int sc_main(int argc, char* argv[]){

    sc_signal<bool> in0_Tb, in1_Tb, in2_Tb, in3_Tb;

    sc_signal<unsigned int> sel_Tb;
    sc_signal <bool> out0_Tb;

    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
    
    //Time resolution of the simulation
    sc_set_time_resolution(1, SC_NS);

    Mux *Mux_inst;
    Mux_inst = new Mux("mux");
    Mux_inst ->sel(sel_Tb);
    Mux_inst ->in0(in0_Tb);
    Mux_inst ->in1(in1_Tb);
    Mux_inst ->in2(in2_Tb);
    Mux_inst ->in3(in3_Tb);
    Mux_inst ->out0(out0_Tb);

    Mux_Tb *Mux_Tb_inst;
    Mux_Tb_inst = new Mux_Tb("mux_tb");
    Mux_Tb_inst ->sel(sel_Tb);
    Mux_Tb_inst ->in0(in0_Tb);
    Mux_Tb_inst ->in1(in1_Tb);
    Mux_Tb_inst ->in2(in2_Tb);
    Mux_Tb_inst ->in3(in3_Tb);
    //Mux_Tb ->out0(out0_Tb);

    //Create a trace file.
    sc_trace_file* tf = sc_create_vcd_trace_file("trace_NAME");
    sc_trace(tf,in0_Tb, "in0");
    sc_trace(tf,in1_Tb, "in1");
    sc_trace(tf,in2_Tb, "in2");
    sc_trace(tf,in3_Tb, "in3");
    sc_trace(tf,sel_Tb, "sel");
    sc_trace(tf,out0_Tb, "out0");
    //Start Simulation
sc_start(500, SC_NS);
cout << " Finish Simulation \n";
sc_close_vcd_trace_file(tf);

    return(0);

}