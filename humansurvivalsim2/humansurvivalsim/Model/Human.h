#pragma once
#include "Health.h"

class Human {

public:

	Human();
	~Human();

	Health* getHealth();

	void setWeight(int);
	int getWeight();

	void calculateWaterRequired();
	int getWaterRequired();

	void updateWaterConsumed();
	int getWaterConsumed();

private:
	Health* health;
	int weight; //Kilograms
	int waterRequired; // How much water needed to stay hydrated in the current situation (considering activity  time and heat)
	int waterConsumed; // How much water consumed in the current day
	int speed; //A mulitplier that will affect how long it takes to perform an action

};

