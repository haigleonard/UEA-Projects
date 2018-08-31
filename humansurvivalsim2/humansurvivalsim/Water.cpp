#include "Model/Water.h"

Water::Water()
{
}


Water::~Water()
{
}

void Water::setUWater(double nUWater)
{
	Water::unfiltered_water = nUWater;
}

double Water::getUWater()
{
	return Water::unfiltered_water;
}

void Water::setFWater(double nFwater)
{
	Water::filtered_water = nFwater;
}

double Water::getFWater()
{
	return Water::filtered_water;
}

bool Water::checkFoundWater()
{
	return Water::foundWater;
}

void Water::setFoundWater(bool newFoundWater)
{
	Water::foundWater = newFoundWater;
}

double Water::getWaterRequired()
{
	return Water::waterRequired;
}


void Water::updateWaterConsumed()
{
	Water::waterConsumed += 230;
}


double Water::getWaterConsumed()
{
	return Water::waterConsumed;
}
