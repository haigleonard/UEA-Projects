#include "Model/Map.h"
#include <time.h>
#include <ctime>
#include <random>
#include <fstream>


Map::Map() {
}

Map::~Map() {
}

Map::Map(std::vector<std::vector<Node*>> copyNodes){	
	for (int i = 0; i < copyNodes.size(); i++)	{
		for (int j = 0; j < copyNodes[i].size(); j++)
		{
			this->nodes[i][j] = copyNodes[i][j];
		}
	}
}

bool Map::getStart() {
	return start;
}

Node* Map::getNode(int i, int e) {
	return nodes[i][e];
}

void Map::iniMap(int x, int y,  float temp, int treeC) {

	start = true;
	treePercent = treeC;
	if (temp == 0.5) {
		humidity = true;
	}
	else {
		humidity = false;
	}
	dayDegr = 7;
	targetTemp = 30;
	tempDif = targetTemp - dayDegr;

	rows = x;
	cols = y;
	
	//Alocate memory and push nodes
	for (int i = 0; i < x; i++) {
		std::vector<Node*> temp;
		for (int j = 0; j < y; j++) {
			Node* node = new Node(i, j);
			temp.push_back(node);
		}
		nodes.push_back(temp);
	}


	srand(time(NULL));

	//Set values to each node based on a random probabilistic manner 
	double multiplier;
	double radius;
	int centerX = floor(x / 2);
	int centerY = floor(y / 2);
	

	for (int j = 0; j < y; j++) {
		for (int i = 0; i < x; i++) {			
			multiplier = 0.90 + (rand() / (RAND_MAX / (1 - 0.90)));
			radius = floor(y / 1.95) * multiplier;
			if (pow(i - centerX, 2) + pow(j - centerY, 2) <= pow(radius, 2)) {
				nodes[i][j]->setValue(1);
			} else {
				nodes[i][j]->setValue(-9);
			}
		}		
	}


	int lowerX = floor(x*0.2);
	int upperX = floor(x*0.8);

	int lowerY = floor(y*0.2);
	int upperY = floor(y*0.8);


	int randX = lowerX + (rand() % (upperX - lowerX + 1));
	int randY = lowerY + (rand() % (upperY - lowerY + 1));

	int numToPrint = 0.1*rows;
	int count = 0;
	
	while (!(count == (0.1*rows))) {
		for (int i = -numToPrint; i < numToPrint + 1; i++) {
			multiplier = rand() % 2;
			if ((i == numToPrint || i == -numToPrint) && multiplier == 1) {
				//do nothing
			}
			else {
				nodes[randX + i][randY + count]->setValue(0);
			}

		}
		numToPrint--;
		count++;
	}

	numToPrint = 0.1*rows;
	count = 0;

	while (!(count == (0.1*rows))) {
		for (int i = -numToPrint; i < numToPrint + 1; i++) {
			multiplier = rand() % 2;
			if ((i == numToPrint || i == -numToPrint) && multiplier == 1) {
				//do nothing
			}
			else {
				nodes[randX + i][randY - count]->setValue(0);
			}

		}
		numToPrint--;
		count++;
	}


	int dir = 4;
	if (rows - randX >= randX && cols - randY > randX && randY > randX) {
		dir = 0; //left
	}
	else if (rows - randX < randX && rows - randX < cols - randY && rows - randX < randY) {
		dir = 1; //right
	}
	else if (cols - randY > randY && randY < rows - randX && randY < randX) {
		dir = 2; //down
	}
	else {
		dir = 3; //up
	}

	switch (dir) {
	case(0): //left
		for (int i = randX; i > 0; i--) {
			nodes[randX - i][randY]->setValue(0);
		}
		break;
	case(1): //rigth
		for (int i = 0; i < rows - randX; i++) {
			nodes[randX + i][randY]->setValue(0);
		}
		break;
	case(2): //up
		for (int i = randY; i > 0; i--) {
			nodes[randX][randY - i]->setValue(0);
		}
		break;
	case(3): //down
		for (int i = 0; i < cols - randY; i++) {
			nodes[randX][randY + i]->setValue(0);
		}
		break;
	default:
		break;
	}

	for (int j = 0; j < x; j++) {
		for (int i = 0; i < y; i++) { 
			int likelihood = rand() % 100 + 1;
			if (likelihood > 0 && likelihood <= (95 - treePercent) && nodes[i][j]->getValue() != -9 || nodes[i][j]->getValue() == 0) {
				
			}
			else if (likelihood > (95 - treePercent) && likelihood <= 95 && nodes[i][j]->getValue() != -9 || nodes[i][j]->getValue() == 0) {
				nodes[i][j]->setValue(-2); treeCount++;
			}
			else {
				if (nodes[i][j]->getValue() != -9 || nodes[i][j]->getValue() == 0) {
					nodes[i][j]->setValue(-1);
				}
			}
		}
	}
}

void Map::clear() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int a = 1;
			nodes[i][j]->setFScore(10000);			
			nodes[i][j]->setGScore(10000);
		}
	}
}

int Map::getNumRows()
{
	return Map::rows;
}

int Map::getNumCols()
{
	return Map::cols;
}


std::vector<int> Map::findValidPos()
{
	
		std::vector<int> result;

		bool validPos = false;
		while (!validPos) {
			int X = rand() % this->getNumRows();
			int Y = rand() % this->getNumCols();
			if (this->getNode(Y, X)->getValue() == 1) {
				result.push_back(X);
				result.push_back(Y);
				validPos = true;
			}
		}

		return result;
	
}

int Map::getTreeCount()
{
	return treeCount;
}

void Map::cutTree() {
	treeCount--;
}

double Map::manhattan_distance(int x1, int y1, int x2, int y2)
{
	double val1 = std::abs(x1 - x2);
	double val2 = std::abs(y1 - y2);
	return val1 + val2;
}
/**********************************************************************************************
* A function to traverse all nodes of the map and to calculate whether the node is an empty
* space (Grass/moveable area). Sets the map variable availableNodes to a vector of Node pointers
* that are all available for an object to move to.
**********************************************************************************************/
std::vector<Node*> Map::getAvailableNodes()
{
	return availableNodes;
}

/**********************************************************************************************
* A function to traverse all nodes of the map and to calculate whether the node is an empty
* space (Grass/moveable area). Sets the map variable availableNodes to a vector of Node pointers
* that are all available for an object to move to.
**********************************************************************************************/
void Map::calculateAvailableNodes()
{
	std::vector<Node*> temp;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {

			if (nodes[i][j]->getValue() == 1) temp.push_back(nodes[i][j]);
		}
	}

	availableNodes = temp;
}

void Map::updateTemperature(int hours, int minutes) 
{
	if (!humidity) {
		if (hours == 14 && minutes >= 58 && minutes < 59) {
			dayDegr = targetTemp;
			targetTemp = rand() % (6 - 1 + 1) + 1; targetTempD = false;

			tempDif = dayDegr - targetTemp;
		}
		if (hours == 6 && minutes >= 58 && minutes < 59) {
			dayDegr = targetTemp;
			targetTemp = rand() % (40 - 28 + 1) + 28;  targetTempD = true;
			tempDif = targetTemp - dayDegr;
		}
		if (targetTempD && minutes >= 58 && minutes < 59) {
			dayDegr += (tempDif / 9);
		}
		if (!targetTempD && minutes >= 58 && minutes < 59) {
			dayDegr -= (tempDif / 15);
		}
	}
	else {
		if (hours == 16 && minutes >= 58 && minutes < 59) {
			dayDegr = targetTemp;
			targetTemp = rand() % (6 - 1 + 1) + 1; targetTempD = false;

			tempDif = dayDegr - targetTemp;
		}
		if (hours == 6 && minutes >= 58 && minutes < 59) {
			dayDegr = targetTemp;
			targetTemp = rand() % (50 - 38 + 1) + 38;  targetTempD = true;
			tempDif = targetTemp - dayDegr;
		}
		if (targetTempD && minutes >= 58 && minutes < 59) {
			dayDegr += (tempDif / 9);
		}
		if (!targetTempD && minutes >= 58 && minutes < 59) {
			dayDegr -= (tempDif / 15);
		}
	}
}

int Map::getDegrees() {
	return dayDegr;
}

void Map::plantTrees() {
	if (((rows*cols)/8)>treeCount) {
		for (int i = 0; i < 4; i++) {
			while (!findtreePosition) {
				int randomX = rand() % rows;
				int randomY = rand() % rows;
				if (nodes[randomX][randomY]->getValue() == 1) {
					nodes[randomX][randomY]->setValue(-2);
					treeCount++;
					findtreePosition = true;
				}
			}
			findtreePosition = false;
		}
	}
}
