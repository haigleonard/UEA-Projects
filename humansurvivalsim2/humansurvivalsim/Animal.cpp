#include "Model/Animal.h"

/********************************************************************************
* Default Constructor
*******************************************************************************/
Animal::Animal() 
{
	health = 0;
	gender = "N/A";
	age = 0;
	hunger = 0;

}
/********************************************************************************
* Constructor - to initiate the provided parameters to a Animal object.
* @param health An integer value that represents the life of an animal, when
* depeleted to zero the animal is classed as death.
* @param gender  A string value that represents the gender of the animal, male or
* female.
* @param age An integer value that represents the time the animal has been alive,
* the larger the number, the longer the animal has existed in the simulation.
* @param hunger An integer value that represents when the animal last ate, zero
* indicates the animal is starving.
*******************************************************************************/
Animal::Animal(int health, std::string gender, int age, int hunger)
{
	Animal::health = health;
	Animal::gender = gender;
	Animal::age = age;
	Animal::hunger = hunger;
}

/********************************************************************************
* Deconstructor
*******************************************************************************/
Animal::~Animal()
{

}

/********************************************************************************
* 
*******************************************************************************/
int Animal::getHealth()
{
	return health;
}

/********************************************************************************
* Assigning an 
* @param health - 
********************************************************************************/
void Animal::setHealth(int health)
{
	Animal::health = health;
}

std::string Animal::getGender()
{
	return gender;
}

void Animal::setGender(std::string gender)
{
	Animal::gender = gender;
}

int Animal::getAge()
{
	return age;
}

void Animal::setAge(int age)
{
	Animal::age = age;
}

int Animal::getHunger()
{
	return hunger;
}

void Animal::setHunger(int hunger)
{
	Animal::hunger = hunger;
}

int Animal::getSpeed()
{
	return speed;
}

void Animal::setSpeed(int speed)
{
	Animal::speed = speed;
}

/***********************************************************************
* A function to traverse a map to calculate the nodes that are currently
* available for moving to.
* @param map - a Map object that is the current map being used for the
* simulation and grants details on each position of the map.
* @return Returns a vector of Node object that contain X and Y coordinates
* for valid movements.
***********************************************************************/
std::vector<Node> Animal::getValidPaths(Map* map)
{
	int mapColumns = map->getNumCols(), mapRows = map->getNumRows();
	std::vector<Node> validPaths = std::vector<Node>();
	Node node;

	for (int i = 0; i < mapRows; i++) {
		for (int j = 0; j < mapColumns; j++) {
			node = map->getNode(i, j);

			if (node.getValue() == 1) {
				validPaths.push_back(node);
			}


		}
	}

	return validPaths;
}

std::vector<Node> Animal::getPath()
{
	return optimalPath;
}

void Animal::setPath(std::vector<Node> path)
{
	optimalPath = path;
}

/********************************************************************************
* A function that determines an animal/sub childrens gender randomly.
* @return A string object that refers to the name of the gender - Male, female and
* unknown (currently).
*********************************************************************************/
std::string Animal::chooseGender()
{
	std::string gender;
	int random = rand() % Gender::NumOfGenders;

	switch (random) {
	case 0:
		gender = "Male";
		break;
	case 1:
		gender = "Female";
		break;
	default:
		gender = "Unknown";
		break;
	}

	return gender;
}