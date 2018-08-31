#pragma once
#include <string> 

class logFile
{
public:
	logFile();
	~logFile();
	void logUpdate(int x);
	void logAverage(float x, float y,float z, float a, float p, float t);
	void taskUpdate(std::string);
private:
	std::string logString;
	std::string taskString;
	std::string prevString;
	float averageHunger = 0;
	float averageHydration = 1000;
	float averageEnergy = 1000;
	float averageWood = 1000;
	float averageTemperature = 1000;
	float averageSpeed = 1000;
	float averagePrey = 1000;
	float averageTrees = 1000;
	float maxHeat = 1000;
	float minHeat = 1000;
};

