#include "Model/SimulationInfo.h"

void SimulationInfo::setPreyFlag(bool nPreyFlag)
{
	SimulationInfo::preyFlag = nPreyFlag;
}

bool SimulationInfo::getPreyFlag()
{
	return SimulationInfo::preyFlag;
}

void SimulationInfo::update(Survivor* newS, Map* newM, Fire* newF, Shelter* newShel, Water* newWater, Health* newHealth, bool nNightBool, bool nMove, bool nPause, int nNumMoves, float nHours, float nMinutes)
{	
	SimulationInfo::survivor = newS;
	SimulationInfo::currentMap = newM;	
	SimulationInfo::currentFire = newF;
	SimulationInfo::currentShelter = newShel;
	SimulationInfo::currentWater = newWater;
	SimulationInfo::move = nMove;
	SimulationInfo::pause = nPause;
	SimulationInfo::nightBool = nNightBool;
	SimulationInfo::numMoves = nNumMoves;
	SimulationInfo::hours = nHours;
	SimulationInfo::minutes = nMinutes;	
	SimulationInfo::currentHealth = newHealth;
}

void SimulationInfo::updatePrey(std::vector<Prey>* newPrey, Map* newM, bool nPause, bool nPreyFlag)
{
	prey = newPrey;
	currentMap = newM;	
	pause = nPause;	
	preyFlag = nPreyFlag;
}

Map* SimulationInfo::getMap() {
	return SimulationInfo::currentMap;
}

Survivor* SimulationInfo::getSurvivor() {
	return SimulationInfo::survivor;
}

void SimulationInfo::setSurvivor(Survivor* newS)
{
	SimulationInfo::survivor = newS;
}

Shelter * SimulationInfo::getShelter()
{
	return SimulationInfo::currentShelter;
}

void SimulationInfo::setShelter(Shelter * newShel)
{
	SimulationInfo::currentShelter = newShel;
}

Water* SimulationInfo::getWater()
{
	return SimulationInfo::currentWater;
}

void SimulationInfo::setWater(Water * nW)
{
	SimulationInfo::currentWater = nW;
}

bool SimulationInfo::getMove() {
	return SimulationInfo::move;
}

int SimulationInfo::getNumMoves() {
	return SimulationInfo::numMoves;
}

void SimulationInfo::setNumMoves(int moves) {
	SimulationInfo::numMoves = moves;
}

void SimulationInfo::setMove(bool nMove)
{
	SimulationInfo::move = nMove;
}

bool SimulationInfo::getPause()
{
	return SimulationInfo::pause;
}

void SimulationInfo::setPause(bool newPause)
{
	SimulationInfo::pause = newPause;
}

bool SimulationInfo::checkNightBool()
{
	return SimulationInfo::nightBool;
}

void SimulationInfo::setNightBool(bool newNightBool)
{
	SimulationInfo::nightBool = newNightBool;
}

float SimulationInfo::getHours()
{
	return SimulationInfo::hours;
}

void SimulationInfo::setHours(float newHours)
{
	SimulationInfo::hours = newHours;
}

float SimulationInfo::getMinutes()
{
	return SimulationInfo::minutes;
}

void SimulationInfo::setMinutes(float newMinutes)
{
	SimulationInfo::minutes = newMinutes;
}

void SimulationInfo::setMap(Map *m) {
	SimulationInfo::currentMap = m;
}

Fire * SimulationInfo::getFire()
{
	return SimulationInfo::currentFire;
}

void SimulationInfo::setFire(Fire* f)
{
	SimulationInfo::currentFire = f;
}

int SimulationInfo::getPreyCount()
{
	return preyCount;
}


void SimulationInfo::setPreyCount(int count)
{
	preyCount = count;
}

void SimulationInfo::setPrey(std::vector<Prey>* listOfPrey)
{
	prey = listOfPrey;
}

std::vector<Prey>* SimulationInfo::getPrey()
{
	return prey;
}


Health * SimulationInfo::getHealth()
{
	return currentHealth;
}

void SimulationInfo::setHealth(Health * currentHealth) {
	SimulationInfo::currentHealth = currentHealth;
}

void SimulationInfo::setCalouriesUsed(double nCaloriesUsed)
{
	SimulationInfo::caloriesUsed = nCaloriesUsed;
}

double SimulationInfo::getCaloriesUsed()
{
	return SimulationInfo::caloriesUsed;
}