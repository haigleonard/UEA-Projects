#pragma once
#include "FSM.h"
#include "SimulationInfo.h"
#include "Health.h"
#include "PathFinder.h"
#include "Node.h"


//class SimulationInfo;
//
//class Survivor
//{
//public:
//	Survivor();	
//	~Survivor();
//	Survivor(const Survivor* s) : xPos(s->xPos), yPos(s->yPos), weight(s->weight), health(s->health){};
//	void setlocationX(float);
//	void setlocationY(float);
//	float getlocationX();
//	float getlocationY();
//	Health* getHealth();	
//	void setWeight(int);
//	int getWeight();	
//	double getEnergy();
//	void setEnergy(double);
//	int getWood();
//	void setWood(int);
//	void setSpeed(float);
//	float getSpeed();
//	void setDehydration(double);
//	double getDehydration();
//	void setTemperature(double);
//	double getTemperature();
//	void setWaterLevel(double);
//	double getWaterLevel();
//	void setCurrDehyd(double);
//	double getCurrDehyd();
//	void iniSur();
//	bool getInjuryStatus();
//	void setInjuryStatus();
//	void setInjurychance(int);
//	int getPreyStock();
//
//
//	//Work related to the finite state machine
//	Survivor(SimulationInfo *);
//	FSM *survivorFSM;
//	typedef enum { wander, run, rest, collect_wood, collect_water, make_shelter, make_fire, returning_home, maintain_fire, check_metrics, filter_water, go_water, kill_prey} State;
//	State currentState;
//	void Wander(SimulationInfo *);
//	void Run(SimulationInfo *);
//	void Rest(SimulationInfo *);
//	void CollectWood(SimulationInfo *);
//	void CollectWater(SimulationInfo *);
//	void MakeShelter(SimulationInfo *);
//	void MakeFire(SimulationInfo *);
//	void ReturningHome(SimulationInfo *);
//	void MaintainFire(SimulationInfo *);
//	void CheckMetrics(SimulationInfo *);
//	void FilterWater(SimulationInfo *);
//	void GoWater(SimulationInfo *);
//	void KillPrey(SimulationInfo *);
//	static void adapter(void *, SimState *, int);
//	//May need some collision detection FSM
//	
//
//private:
//	int wood;	
//	int timer;
//	float xPos;
//	float yPos;
//	Health* health;
//	int weight; //Kilograms
//	int preyStock = 1;
//	float speed; //A mulitplier that will affect how long it takes to perform an action	
//	double energy;
//	double dehydration;
//	double curr_dehydration;	
//	double temperature;
//	double waterLevel;
//	bool injuryStatus = false;
//	int injuryChance = 200;
//	std::vector<Node> optimal_path; 
//	std::vector<Node> pathToHome;
//	std::vector<Node> pathToWater;
//	PathFinder finder;
//	Node startNode;
//	Node goalNode;
//};
class SimulationInfo;

class Survivor
{
public:
	Survivor();
	~Survivor();
	Survivor(const Survivor* s) : xPos(s->xPos), yPos(s->yPos), weight(s->weight), health(s->health) {};
	void setlocationX(float);
	void setlocationY(float);
	float getlocationX();
	float getlocationY();
	Health* getHealth();
	void setWeight(double);
	double getWeight();
	double getEnergy();
	void setEnergy(double);
	int getWood();
	void setWood(int);
	void setPreyKilled(int);
	int getPreyKilled();
	void setSpeed(double);
	double getSpeed();
	void setDehydration(double);
	double getDehydration();
	void setTemperature(double);
	double getTemperature();
	void setWaterLevel(double);
	double getWaterLevel();
	void setCurrDehyd(double);
	int getTask();
	double getCurrDehyd();
	double calculateBMR();
	void iniSur();
	bool getInjuryStatus();
	void setInjuryStatus();
	void setSTask(std::string);
	std::string getSTask();
	double weigthLoss();
	void setInitialWeight(double);
	double getInitialWeight();
	//Work related to the finite state machine
	Survivor(SimulationInfo *);
	FSM *survivorFSM;
	typedef enum {
		wander, run, rest, collect_wood, collect_water, returning_home,
		check_metrics, filter_water, go_water, decision, cooking
	} State;
	State currentState;
	void Wander(SimulationInfo *);
	void Run(SimulationInfo *);
	void Rest(SimulationInfo *);
	void CollectWood(SimulationInfo *);
	void CollectWater(SimulationInfo *);
	void MakeShelter(SimulationInfo *);
	void MakeFire(SimulationInfo *);
	void ReturningHome(SimulationInfo *);
	void Decision(SimulationInfo *);
	void CheckMetrics(SimulationInfo *);
	void FilterWater(SimulationInfo *);
	void GoWater(SimulationInfo *);
	void Cooking(SimulationInfo *);
	static void adapter(void *, SimState *, int);


	
	//May need some collision detection FSM


private:
	int wood;
	int woodStock;
	bool woodTask;
	int preyStock;
	bool preyTask;
	bool shelterTask;
	bool dayEnded;
	int timer;
	float xPos;
	float yPos;
	Health* health;
	double speed; //A mulitplier that will affect how long it takes to perform an action	
	double energy;
	bool injuryStatus = false;
	int injuryChance = 100;
	double dehydration;
	double curr_dehydration;
	double temperature;
	double waterLevel;
	double height; //in cm
	double weight; //in kilograms
	double bmr;
	double caloriesMul;
	double calorieIn;
	double initialWeight;
	int age;
	int task;
	std::string sTask;
	double bmrIncrease;
	std::unordered_map<int, int> validGoals;
	std::vector<Node> optimal_path;
	std::vector<Node> pathToHome;
	std::vector<Node> pathToWater;
	PathFinder finder;
	Node startNode;
	Node goalNode;
};