#include "Model/Prey.h"

SimState * Info_Sim;

Prey::Prey(SimulationInfo * info)// : Animal(health, gender, age, hunger)
{	
	//Set other parameters here

	//Creating FSM and setting the initial state
	preyFSM = new FSM();
	Info_Sim = info;
	this->preyFSM->setState(&adapter, this, info, this->wander);
	currentState = rest;
	currentMove = 1;
	info->setPrey(false);
}


Prey::Prey()
{
}

Prey::~Prey()
{
}

/***********************************************************************
* A function to traverse a map to calculate the nodes that are currently
* available for moving to, it doesn't return all free valid moves,
* restricted to a smaller area.
* @param map - a Map object that is the current map being used for the
* simulation and grants details on each position of the map.
* @return Returns a vector of Node object that contain X and Y coordinates
* for valid movements.
***********************************************************************/
std::vector<Node> Prey::getValidGoalNodes(SimulationInfo* mapInfo)
{
	int multipler = 1;
	Node* node, currentPos = this;
	std::vector<Node> validPaths = std::vector<Node>();
	int size = (mapInfo->getMap()->getNumRows() * mapInfo->getMap()->getNumCols()) * 0.002; // Calculating 10% of the maps nodes.
	bool areaCovered = false, check = false;

	while (!areaCovered) {

		for (int i = -1 * multipler; i < 1 + multipler; i++) {
			for (int j = -1 * multipler; j < 1 + multipler; j++) {

				if (multipler == i || -multipler == i || -multipler == j || multipler == j) {
					check = (this->getX() + i >= 0 && this->getY() + j >= 0) && (this->getX() + i < mapInfo->getMap()->getNumRows() && this->getY() + j < mapInfo->getMap()->getNumCols());

					if (check) {
						node = mapInfo->getMap()->getNode(this->getX() + i, this->getY() + j);
						if (node->getValue() == 1 && node->getX() != this->getX() && node->getY() != this->getY()) validPaths.push_back(node); //Adding node if it's grass (1) and not the starting position of prey object.
					}
				}

			}

			if (size-- == 0) areaCovered = true; //Decrementing size until the initial total of size becomes zero.
		}

		if (areaCovered) return validPaths; //An early break to avoid any other nodes being added to vector.

		multipler++;
	}

	return validPaths;
}

/***********************************************************************
*
***********************************************************************/
void Prey::Wander(SimulationInfo * info)
{
	//if (!info->getPause()) {
	//	if (currentMove != this->prey_path.size() + 1) {
	//		if (currentMove != 1) {
	//			info->getMap()->getNode(this->prey_path[this->prey_path.size() - currentMove + 1].getX(), this->prey_path[this->prey_path.size() - currentMove + 1].getY())->setValue(1);
	//		}
	//		info->getMap()->getNode(this->prey_path[this->prey_path.size() - currentMove].getX(), this->prey_path[this->prey_path.size() - currentMove].getY())->setValue(3);
	//		this->setX(this->prey_path[this->prey_path.size() - currentMove].getX());
	//		this->setY(this->prey_path[this->prey_path.size() - currentMove].getY());

	//		currentMove++;
	//	}
	//	else {
	//		currentMove = 1;
	//		info->getMap()->clear(); //Resetting scores for nodes of the map
	//		std::vector<Node> validGoalNodes = this->getValidGoalNodes(info);
	//		Node goal = validGoalNodes.at(rand() % validGoalNodes.size());
	//		this->prey_path = this->preyFinder.path(this, goal, info->getMap());
	//	}
	//}

	if (!info->getPause()) {
		
		if (!(info->checkNightBool())) {			
			currentState = rest;
		}

		if (currentMove != this->prey_path.size() + 1) {
			if (currentMove != 1) {
				info->getMap()->getNode(this->prey_path[this->prey_path.size() - currentMove + 1].getX(), this->prey_path[this->prey_path.size() - currentMove + 1].getY())->setValue(1);
			}

			//Need to check if the current path crosses with the shelter
			if ((this->prey_path[this->prey_path.size() - currentMove].getX() == info->getShelter()->getX() && this->prey_path[this->prey_path.size() - currentMove].getY() == info->getShelter()->getY())
				||
				(this->prey_path[this->prey_path.size() - currentMove].getX() == info->getWater()->getX() && this->prey_path[this->prey_path.size() - currentMove].getY() == info->getWater()->getY())
				||
				(this->prey_path[this->prey_path.size() - currentMove].getX() == info->getFire()->getX() && this->prey_path[this->prey_path.size() - currentMove].getY() == info->getFire()->getY())
				||
				(this->prey_path[this->prey_path.size() - currentMove].getX() == floor(info->getSurvivor()->getlocationX()) && this->prey_path[this->prey_path.size() - currentMove].getX() == ceil(info->getSurvivor()->getlocationX())
					&& this->prey_path[this->prey_path.size() - currentMove].getY() == floor(info->getSurvivor()->getlocationY()) && this->prey_path[this->prey_path.size() - currentMove].getY() == ceil(info->getSurvivor()->getlocationY()))) {
				this->prey_path.clear();
				currentMove = this->prey_path.size() + 1;
			}
			else {
				info->getMap()->getNode(this->prey_path[this->prey_path.size() - currentMove].getX(), this->prey_path[this->prey_path.size() - currentMove].getY())->setValue(3);
				this->setX(this->prey_path[this->prey_path.size() - currentMove].getX());
				this->setY(this->prey_path[this->prey_path.size() - currentMove].getY());
				currentMove++;
			}

			if (currentMove == this->prey_path.size()) {
				info->setPreyFlag(false);
			}

		}
		else {		
			
			Node s = Node(this->x, this->y);
		
			bool validGoal = false;
			int range = 3;
			int xLowerBound = this->x - range;
			int yLowerBound = this->y - range;
			int xUpperBound = this->x + range;
			int yUpperBound = this->y + range;
			int xG;
			int yG;

			Node g;

			while (!validGoal) {
				xG = xLowerBound + (std::rand() % (xUpperBound - xLowerBound + 1));
				yG = yLowerBound + (std::rand() % (yUpperBound - yLowerBound + 1));
				bool check = xG >= 0 && yG >= 0 && xG < info->getMap()->getNumRows() && yG < info->getMap()->getNumCols();
				if (check) {
					if (info->getMap()->getNode(xG, yG)->getValue() == 1 && this->x != xG && this->y != yG) {
						g.setX(xG);
						g.setY(yG);						
						validGoal = true;
					}
				}
			}
			//Resetting scores for nodes of the map and computing a new path
			info->getMap()->clear();
		
			//Need to set the last position of prey to a valid node
			if (this->prey_path.size() > 0) { //size check for first run
				info->getMap()->getNode(this->prey_path[0].getX(), this->prey_path[0].getY())->setValue(1);
			}			
			
			//std::vector<Node> validGoalNodes = this->getValidGoalNodes(info);
			//Node goal = validGoalNodes.at(rand() % validGoalNodes.size());
			this->prey_path = this->preyFinder.path(s, g, info->getMap());
			currentMove = 1;						
		}
	}
}


void Prey::Rest(SimulationInfo * info)
{
	if (!(info->checkNightBool())) {
		currentState = rest;
	}
	else {
		if (info->getPreyFlag()) {
			currentState = wander;
		} else {
			currentState = rest;
		}
		
	}
}

/***********************************************************************
*
***********************************************************************/
void Prey::adapter(void * context, SimState * info, int state)
{
	switch ((State)state) {
	case wander: static_cast<Prey *>(context)->Wander((SimulationInfo *)info); break;
	case rest: static_cast<Prey *>(context)->Rest((SimulationInfo *)info); break;
	default: static_cast<Prey *>(context)->Wander((SimulationInfo *)info); break;
	}
}

/********************************************************************************
* A set function that updates the current move of a prey object.
* @param moveNumber An integer value that is used to track the number of moves a
* prey object has executed through it's optimal path.
********************************************************************************/
void Prey::setCurrentMove(int moveNumber)
{
	currentMove = moveNumber;
}
/********************************************************************************
* A get function that retrieves the current move integer value that refers to the
* index move in optimal path.
********************************************************************************/
int Prey::getCurrentMove()
{
	return currentMove;
}