#include "Model/Shelter.h"



Shelter::Shelter()
{
}


Shelter::~Shelter()
{
}

void Shelter::setMadeCamp(bool newCamp)
{
	Shelter::madeCamp = newCamp;
}

bool Shelter::getCamp()
{
	return Shelter::madeCamp;
}
