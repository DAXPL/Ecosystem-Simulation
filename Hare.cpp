#include "Hare.h"
#include <iostream>

Hare::Hare()
{
	this->isMale = (rand() % 2)==0;
	this->food = (rand() % 10) + 15;
	this->furGenotype[0] = (rand() % 4);
	this->furGenotype[1] = (rand() % 4);
	this->age = (rand() % 20)+10;
}

void Hare::PrintOutHare()
{
	std::cout << this << " " << isMale << " " << food << std::endl;
}

void Hare::SimulateHare()
{
	food--;
	age++;
}

bool Hare::IsAlive()
{
	return food > 0 && age <= 1825; //5lat
}

bool Hare::IsHareMale()
{
	return this->isMale;
}

bool Hare::IsChild()
{
	return age<30;
}

int Hare::GetHareFurFenotype()
{
	if (furGenotype[0] == 3 || furGenotype[1] == 3) return 3; //allel "czarny"
	if (furGenotype[0] == 2 || furGenotype[1] == 2) return 2; //allel "szynszylowy"
	if (furGenotype[0] == 1 || furGenotype[1] == 1) return 1; //allel "himalajski"
	return 0; //allel "albinotyczny"
}

void Hare::HaveSex(Hare* partner)
{
	if (partner == nullptr || partner->IsHareMale() == this->IsHareMale() || partner==this)
	{
		std::cerr << "You cant mate with null partner, same gender hare or yourself" << std::endl;
		return;
	}
}