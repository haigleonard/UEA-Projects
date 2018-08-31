#include "Model/Health.h"



Health::Health() : hungerTotal(1000)
{
	Health::hunger = 1000;
	Health::dead = false;
	calculateHungerStatus();

}


Health::~Health() 
{
}

/********************************************************************************
* A method to calculate the Human object (Surviviors) current hunger level and
* assign it to a string field to be a clear representation to the user of the 
* program on how hungry the survivior is.
********************************************************************************/
void Health::calculateHungerStatus()
{
	int hunGaugeLevel = Health::hungerTotal / 5;

	if (Health::hunger >= 0 && Health::hunger < hunGaugeLevel) {
		Health::hungerStatus = "Too Hungry";
	}
	else if (Health::hunger >= hunGaugeLevel && Health::hunger < (hunGaugeLevel * 2)) {
		Health::hungerStatus = "Hungry";
	}
	else if (Health::hunger >= (hunGaugeLevel * 2) && Health::hunger < (hunGaugeLevel * 3)) {
		Health::hungerStatus = "Neutral";
	}
	else if (Health::hunger >= (hunGaugeLevel * 3) && Health::hunger < (hunGaugeLevel * 4)) {
		Health::hungerStatus = "Satified";
	}
	else {
		Health::hungerStatus = "Too Full";
	}
	
}

void Health::setHunger(int hunger)
{
	if (hunger <= 0) { Health::hunger = 0; dead = true; }
	if (hunger > 0 && dead == false) { Health::hunger = hunger; }
	calculateHungerStatus();
}

int Health::getHunger()
{
	return Health::hunger;
}

std::string Health::getHungerStatus()
{
	return Health::hungerStatus;
}

void Health::calculateDehydrationLevel()
{
}

int Health::getDehydrationLevel()
{
	return 0;
}

void Health::setInjury(std::string)
{
}

std::string Health::getInjury()
{
	return std::string();
}

bool Health::checkDead()
{
	return Health::dead;
}

bool Health::getIllness() {
	return illNess;
}

void Health::setIllness(bool fire, bool rain, bool night) {
	if (fire && rain && night && !illNess) {
		illNess = true;
	}
}

void Health::recoveryProcess(float food, float hydration, float nights) {
	if (illNess) {
		foodRecovery += food;
		hydrationRecvoery += hydration;
		restRecovery += nights;
	}
	if (foodRecovery == 5000 && hydrationRecvoery == 5000 && restRecovery == 5) {
		illNess = false;
	}
}