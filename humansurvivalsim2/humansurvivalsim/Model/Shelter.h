#pragma once
#include "Node.h"
class Shelter :
	public Node
{
public:
	Shelter();
	Shelter(const Shelter* s) : Node(s), madeCamp(s->madeCamp){};
	~Shelter();
	void setMadeCamp(bool);
	bool getCamp();

private:
	bool madeCamp = false;	
};