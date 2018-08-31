#include "Model/FSM.h"
#include <iostream>

// Finite State Machine function definitions

FSM::FSM()
{

}

void FSM::setState(void(*fp)(void *), void *context)
{
	fpoint = fp;
}


void FSM::setState(void(*fp)(void *, SimState *, int), void *, SimState *, int)
{
	fpoint2 = fp;
}

void FSM::update(void *context)
{
	if (fpoint != 0)fpoint(context);
}

void FSM::update(void *context, SimState *info, int value)
{
	if (fpoint2 != 0)fpoint2(context, info, value);
}
