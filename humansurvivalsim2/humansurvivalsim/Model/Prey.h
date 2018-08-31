#pragma once
#include "Animal.h"
#include "SimulationInfo.h"
#include "PathFinder.h"
#include <string>

//class Prey :
//	public Animal
//{
//public:
//	Prey();
//	~Prey();	
//	Prey(const Prey* p) : currentMove(p->currentMove), preyFinder(p->preyFinder), prey_path(p->prey_path) {};
//	std::vector<Node> getValidGoalNodes(SimulationInfo* map);
//
//	Prey(SimulationInfo*);
//	FSM *preyFSM;
//	typedef enum { wander, dead } State;
//	State currentState;
//	void Wander(SimulationInfo *);
//	
//	static void adapter(void *, SimState *, int);
//	
//	void setCurrentMove(int);
//	int getCurrentMove();
//
//private:
//	int currentMove;
//	PathFinder preyFinder;
//	std::vector<Node> prey_path;
//};
class SimulationInfo;

class Prey :
	public Animal
{
public:
	Prey();
	~Prey();
	Prey(const Prey* p) : currentMove(p->currentMove), preyFinder(p->preyFinder), prey_path(p->prey_path) {};
	std::vector<Node> getValidGoalNodes(SimulationInfo* map);

	Prey(SimulationInfo*);
	FSM *preyFSM;
	typedef enum { wander, rest } State;
	State currentState;
	void Wander(SimulationInfo *);
	void Rest(SimulationInfo *);
	static void adapter(void *, SimState *, int);

	void setCurrentMove(int);
	int getCurrentMove();
	void setState(State);

private:
	int currentMove;
	PathFinder preyFinder;
	std::vector<Node> prey_path;
};

