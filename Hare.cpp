#include "Hare.h"
#include <iostream>

int GetSimulationID();
int GetNewHareID();

Hare::Hare()
{
	this->hareID = GetNewHareID();

	this->isMale = (rand() % 2)==0;

	this->food = (rand() % 1500) + 600;
	this->foodUsage = (rand() % 50) + 100;

	this->furGenotype[0] = (rand() % 4);
	this->furGenotype[1] = (rand() % 4);
	this->age = (rand() % 20)+10;
}

Hare::Hare(int genA, int genB, int mother, int father)
{
	this->hareID = GetNewHareID();

	this->isMale = (rand() % 2) == 0;

	this->food = (rand() % 1500) + 600;
	this->foodUsage = (rand() % 50) + 100;

	this->furGenotype[0] = genA;
	this->furGenotype[1] = genB;

	this->hareMotherID = mother;
	this->hareFatherID = father;

	this->age = 0;
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
	daySinceLastMate++;
	alreadyMoveThisDay = false;
}

int Hare::ManagePregnacy()
{
	if (isPregnant)
	{
		pregnacyTimeLeft--;

		if (pregnacyTimeLeft <= 0)
		{
			isPregnant = false;
			pregnacyTimeLeft = 0;
			return (int)((rand() % 4)+1);
		}
	}
	return 0;
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

bool Hare::IsReadyToProcreate()
{
	return (food>=foodUsage) && (!isPregnant) && (daySinceLastMate>2) && (IsAlive() && (!IsChild()));
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
	//if (alreadyMoveThisDay || isPregnant) vec = 0;
	if (alreadyMoveThisDay) vec = 0;
	alreadyMoveThisDay = true;

	return vec;
}

void Hare::HaveSex(Hare* partner)
{
	if (partner == nullptr || partner->IsChild() == true || partner->IsHareMale() == this->IsHareMale() || partner == this || partner->IsAlive() == false)
	{
		std::cerr << "You cant mate with null partner, kid, same gender hare or yourself" << std::endl;
		return;
	}
	daySinceLastMate = 0;

	if (isMale == false)
	{
		isPregnant = true;
		pregnacyTimeLeft = pregnacyTime;
		this->fatherfurGenotype[0] = partner->furGenotype[0];
		this->fatherfurGenotype[1] = partner->furGenotype[1];
		this->partnerID = partner->hareID;
	}
}

void Hare::WriteHareToDatabase(sqlite3* db)
{
	std::string sAge = std::to_string(age);
	std::string sSim = std::to_string(GetSimulationID());
	std::string sID = std::to_string(hareID);
	std::string sF0 = std::to_string(furGenotype[0]);
	std::string sF1 = std::to_string(furGenotype[1]);
	std::string sGender = std::to_string((int)(IsHareMale()));
	std::string sMother= std::to_string(hareMotherID);
	std::string sFather = std::to_string(hareFatherID);
	std::string sAlive = std::to_string((int)(IsAlive()));

	std::string insertSql = "INSERT INTO Hares (age, simulation, simulationID, fenotype0, fenotype1, gender, mother, father, isAlive)"
		"VALUES (" + sAge + ", "  + sSim +", " +sID+ ", " + sF0 + ", " + sF1 + ", " + sGender + ", " + sMother + ", " + sFather + ", " + sAlive + ")";

	if (sqlite3_exec(db, insertSql.c_str(), 0, 0, 0) != SQLITE_OK)
	{
		std::cerr << "B³¹d przy dodawaniu zaj¹ca: " << sqlite3_errmsg(db) << std::endl;
		std::cerr << insertSql << std::endl;
	}
}

