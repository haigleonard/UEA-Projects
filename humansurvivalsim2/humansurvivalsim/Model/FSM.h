#pragma once
#ifndef FSM_H
#define FSM_H

// Finite State Machine class definition

class SimState
{
	//SimState() {}// base class of which any simulation information inherits
};

class FSM
{
public:
	FSM();
	void setState(void(*fpnt)(void *), void *);
	void setState(void(*fpnt)(void *, SimState *, int), void *, SimState *, int);
	void update(void *);
	void update(void *, SimState *, int);	
	void(*fpoint)(void *);
	void(*fpoint2)(void *, SimState *, int);
};


#endif