#include "Hare.h"
#include <iostream>

Hare::Hare()
{
	this->isMale = (rand() % 2)==0;

	this->food = (rand() % 1500) + 600;
	this->foodUsage = (rand() % 50) + 100;

	this->furGenotype[0] = (rand() % 4);
	this->furGenotype[1] = (rand() % 4);
	this->age = (rand() % 20)+10;
}

void Hare::PrintOutHare()
{
	std::cout << this << " " << isMale << " " << food << std::endl;
}

void Hare::SimulateHare(int* tileFood, int maxFood)
{
	int caloriesToEat = maxFood;
	if (caloriesToEat > *tileFood) caloriesToEat = *tileFood;
	if (caloriesToEat > maxFoodPerDay) caloriesToEat = maxFoodPerDay;
	if (food > maxFatSize) caloriesToEat = 0;

	food -= foodUsage;
	*tileFood -= caloriesToEat;

	food += caloriesToEat;
	
	age++;
	alreadyMoveThisDay = false;
}

bool Hare::IsAlive()
{
	return food > 0 && age <= 1825; //5lat
	//return true;//to debug movement
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

std::string Hare::GetHareFurFenotypeName()
{
	if (furGenotype[0] == 3 || furGenotype[1] == 3) return std::string("Czarny"); //allel "czarny"
	if (furGenotype[0] == 2 || furGenotype[1] == 2) return std::string("Szynszylowaty"); //allel "szynszylowy"
	if (furGenotype[0] == 1 || furGenotype[1] == 1) return std::string("Himalajski"); //allel "himalajski"
	return std::string("Albinotyczny"); //allel "albinotyczny"
}
//0-stay, 1-left, 2-up, 3-right, 4-down
int Hare::GetMoveVector()
{
	int vec = rand() % 4;
	if (alreadyMoveThisDay) vec = 0;
	alreadyMoveThisDay = true;

	return vec;
}

void Hare::HaveSex(Hare* partner)
{
	if (partner == nullptr || partner->IsChild() == true || partner->IsHareMale() == this->IsHareMale() || partner == this)
	{
		std::cerr << "You cant mate with null partner, kid, same gender hare or yourself" << std::endl;
		return;
	}
}