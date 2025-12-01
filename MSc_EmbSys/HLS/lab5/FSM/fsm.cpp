#include <systemc.h>
#include "fsm.h"

void fsm::Update_State() {
	if (reset.read() == true)
	CURRENT_STATE = STATE_Reset;
	else
	CURRENT_STATE = NEXT_STATE;
};
void fsm::Next_State_Proc() {
	open.write(0);
	
	switch (CURRENT_STATE) {
	case STATE_Reset:
	//RESET State
	if(a.read() == 1)
	NEXT_STATE = STATE_A;
	else if(b.read() ==1 )
	NEXT_STATE = STATE_B;
	else
	NEXT_STATE = STATE_Reset;
	break;
	case STATE_A:
	//State A
	if(a.read() == 1)
	NEXT_STATE = STATE_AA;
	else if(b.read()==1)
	NEXT_STATE = STATE_AB;
	else
	NEXT_STATE = STATE_A;
	break;
	case STATE_B:
	//State B
	if(a.read() == 1)
	NEXT_STATE = STATE_AB;
	else if(b.read()==1)
	NEXT_STATE = STATE_BB;
	else
	NEXT_STATE = STATE_B;
	break;
	...
	default:
	NEXT_STATE = CURRENT_STATE;
}
};
