#ifndef FSM_H
#define FSM_H

#include <systemc.h>

#define STATE_Reset 0
#define STATE_A 1
#define STATE_AA 2
...
SC_MODULE(fsm){
	int CURRENT_STATE;
	int NEXT_STATE;
	public:
	sc_in< bool > a;
	sc_in< bool > b;
	sc_in< bool > clk;
	sc_in< bool > reset;
	sc_out< bool > open;
	SC_CTOR(fsm){
		SC_METHOD(Next_State_Proc);
			sensitive << clk.pos();
			reset_signal_is(reset, true);
		SC_METHOD(Update_State);
			sensitive << clk.pos();
	}
	void Update_State();
	void Next_State_Proc();
};

#endif
