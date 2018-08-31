#include "Model/Fire.h"

Fire::Fire()
{
}


Fire::~Fire()
{
}

double Fire::getFireLevel()
{
	return Fire::fireLevel;
}

void Fire::setFireLevel(double newFireLevel)
{
	Fire::fireLevel = newFireLevel;
}

bool Fire::checkFireMade()
{
	return Fire::fireMade;
}

void Fire::setFireMade(bool newFireMade)
{
	Fire::fireMade = newFireMade;
}

