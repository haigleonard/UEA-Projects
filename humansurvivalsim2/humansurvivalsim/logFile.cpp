#include "Model/logFile.h"
#include <iostream>
#include <fstream>
#include <string> 

logFile::logFile()
{
}


logFile::~logFile()
{
}

void logFile::logAverage(float x, float y, float z, float a, float p, float t) {
	averageHunger += x;
	averageHydration += y;
	averageEnergy += z;
	averageWood += a;
	averagePrey += p;
	averageTrees += t;
}

void logFile::logUpdate(int x) {

	logString += "Day: " + std::to_string(x) + 
		"\nweight: " + std::to_string(averageHunger / 600) + 
		"\nHydration: " + std::to_string(averageHydration / 600) +
		"\nEnergy: " + std::to_string(averageEnergy / 600) +
		"\nWood: " + std::to_string(averageWood / 600) +
		"\nPrey: " + std::to_string(averagePrey / 600) +
		"\nTrees: " + std::to_string(averageTrees / 600) +
		"\n\n";
	
	std::ofstream myfile;
	myfile.open("SimulatorLog.txt");
	myfile << logString;
	myfile.close();
	averageHunger = 0;
}

void logFile::taskUpdate(std::string task) {

	if (task != prevString) {
		taskString += task + "\n";
		std::ofstream myfile;
		myfile.open("TaskLog.txt");
		myfile << taskString;
		myfile.close();
		averageHunger = 0;
		prevString = task;
	}
}