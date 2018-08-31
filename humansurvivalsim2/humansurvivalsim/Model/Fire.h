#pragma once
#include "Node.h"
class Fire :
	public Node
{
public:
	Fire();
	Fire(const Fire* f) : Node(f), fireLevel(f->fireLevel), fireMade(f->fireMade){};
	~Fire();	
	double getFireLevel();
	void setFireLevel(double);
	bool checkFireMade();
	void setFireMade(bool);
	

private:	
	double fireLevel;
	bool fireMade;
};

