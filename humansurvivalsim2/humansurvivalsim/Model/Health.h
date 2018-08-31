#pragma once
#include <string>
class Health
{
public:
	Health();
	~Health();
	void calculateHungerStatus();
	void setHunger(int);
	int getHunger();
	std::string getHungerStatus();

	void calculateDehydrationLevel();
	int getDehydrationLevel();

	void setInjury(std::string);
	std::string getInjury();
	bool checkDead();

	bool getIllness();
	void setIllness(bool, bool, bool);
	void recoveryProcess(float, float, float);


private:
	const int hungerTotal; //Max range (Fully satisfied - not hungry)
	int hunger;
	std::string hungerStatus;
	bool dead = false;
	bool illNess = false;
	float foodRecovery = 0;
	float hydrationRecvoery = 0;
	float restRecovery = 0;
	int dehydration; //A percentage determined by waterRequired and waterConsumed - Human Class
	std::string injury; //Name of injury to be used for hindering speed etc

};

