#include "Hare.h"
#include <iostream>

Hare::Hare()
{
	this->isMale = (rand() % 2)==0;
	this->food = (rand() % 10) + 15;
	this->furGenotype[0] = (rand() % 3);
	this->furGenotype[1] = (rand() % 3);
}

void Hare::PrintOutHare()
{
	std::cout << this << " " << isMale << " " << food << std::endl;
}

void Hare::SimulateHare()
{
	food--;
}

bool Hare::IsAlive()
{
	return food>0;
}

bool Hare::IsHareMale()
{
	return this->isMale;
}
