#pragma once

class Survivor;
class Prey;
#include "Model/FSM.h"
#include "Model/Survivor.h"
#include "Model/Prey.h"
#include "Model/Map.h"
#include "Model/Fire.h"
#include "Model/Shelter.h"
#include "Model/Water.h"
#include "Model/Health.h"


class SimulationInfo : public SimState
{
public:

	SimulationInfo() {}

	//Should be parsing and updating data to the Survivor, Predator and Prey in order to update their FSMs
	//Is there a need to have multiple Simulation Info classes. One for each class making use of the FSM template class.
	
	Map* getMap();
	void setMap(Map*);
	Fire* getFire();
	void setFire(Fire*);
	Survivor* getSurvivor();
	void setSurvivor(Survivor*);
	Shelter* getShelter();
	void setShelter(Shelter*);	
	Water* getWater();
	void setWater(Water*);
	int getNumMoves();
	void setNumMoves(int);
	bool getMove();
	void setMove(bool);
	bool getPause();
	void setPause(bool);
	bool checkNightBool();
	void setNightBool(bool);
	float getHours();
	void setHours(float);
	float getMinutes();
	void setMinutes(float);
	float getCurrHour();
	void setCurrHour(float);
	void setPreyFlag(bool);
	bool getPreyFlag();
	void update(Survivor*, Map*, Fire*, Shelter*, Water*, Health*, bool, bool, bool, int, float, float);
	void updatePrey(std::vector<Prey>*, Map*, bool, bool);
	int getPreyCount();
	void setPreyCount(int);
	void setPrey(std::vector<Prey>*);
	std::vector<Prey>* getPrey();
	Health* getHealth();
	void setHealth(Health*);
	void setCalouriesUsed(double);
	double getCaloriesUsed();

private:
	Survivor* survivor;
	Map* currentMap;
	Fire* currentFire;
	Shelter* currentShelter;
	Water* currentWater;
	std::vector<Prey>* prey;
	double caloriesUsed;
	bool move = false;
	bool pause = false;
	bool nightBool;
	float hours; 
	float minutes;
	int numMoves = 1;	
	int preyCount;
	Health* currentHealth;
	bool preyFlag;
};