#pragma once
#include "Model/Node.h"
#include<vector>

class Map
{
public:

	Map();
	~Map();
	Map(std::vector<std::vector<Node *>>);
	void iniMap(int x, int y,  float temp, int treeC);
	Node* getNode(int, int);
	void clear();
	int getNumRows();
	int getNumCols();
	int getDegrees();
	void updateTemperature(int hours, int minutes);
	void plantTrees();
	std::vector<int> findValidPos();	
	Map(const Map* a) : rows(a->rows), cols(a->cols), nodes(a->nodes) {};
	int getTreeCount();
	double manhattan_distance(int, int, int, int);
	bool getStart();
	std::vector<Node*> getAvailableNodes();
	void calculateAvailableNodes();
	void cutTree();
	
private:
	bool findtreePosition = false;
	int treeCount = 0;
	int rows;
	int cols;
	float dayDegr;
	float targetTemp;
	bool targetTempD = true;
	float tempDif;
	bool start = false;
	int treePercent;
	bool humidity;
	std::vector<std::vector<Node*>> nodes;
	std::vector<Node*> availableNodes; // Nodes an object can potentially move to.
};


