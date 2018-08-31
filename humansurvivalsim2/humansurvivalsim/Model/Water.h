#pragma once
#include "Node.h"

class Water :
	public Node
{
public:
	Water();
	Water(const Water* w) : Node(w), unfiltered_water(w->unfiltered_water), filtered_water(w->filtered_water), waterRequired(w->waterRequired), waterConsumed(w->waterConsumed), foundWater(w->foundWater) {};
	~Water();	
	double getWaterRequired();
	void updateWaterConsumed();
	double getWaterConsumed();
	void setUWater(double);
	double getUWater();
	void setFWater(double);
	double getFWater();
	bool checkFoundWater();
	void setFoundWater(bool);


private:
	double unfiltered_water;
	double filtered_water;
	double waterRequired; // How much water needed to stay hydrated in the current situation (considering activity  time and heat)
	double waterConsumed; // How much water consumed in the current day
	bool foundWater;
};

