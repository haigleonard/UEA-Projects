#pragma once
#include <string>
#include <vector>
#include "Model/Map.h"
#include "Model/Node.h"

class Animal : 
	public Node
{
public:
	Animal();
	Animal(int, std::string, int, int);
	~Animal();
	
	int getHealth();
	void setHealth(int);

	std::string getGender();
	void setGender(std::string);

	int getAge();
	void setAge(int);

	int getHunger();
	void setHunger(int);

	int getSpeed();
	void setSpeed(int);

	virtual std::vector<Node> getValidPaths(Map* map);
	std::vector<Node> getPath();
	void setPath(std::vector<Node>);
	static enum Gender { Male = 0, Female = 1, NumOfGenders } Gender;
	static std::string chooseGender(); //Static - used when creating animal/sub children objects.

protected:
	int health;
	std::string gender;
	int age;
	int hunger;
	int speed;
	std::vector<Node> optimalPath;


};

