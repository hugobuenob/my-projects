#include <systemc.h>
#include "mux_Tb.h"

void Mux_Tb::Mux_stimuli(){
    while(true){
        in0.write(false);
        in1.write(false);
        in2.write(false);
        in3.write(false);
        sel.write(0);
        wait(10,SC_NS);
        in0.write(true);
        in1.write(false);
        in2.write(false);
        in3.write(false);
        sel.write(0);
        wait(10,SC_NS);
        in0.write(true);
        in1.write(true);
        in2.write(false);
        in3.write(false);
        sel.write(1);
        wait(10,SC_NS);
        in0.write(true);
        in1.write(true);
        in2.write(false);
        in3.write(false);
        sel.write(1);
        wait(10,SC_NS);


    }
}