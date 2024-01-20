#pragma once
#include <string>
#include <sqlite3.h>
class Hare
{
public:
	Hare();
	Hare(int genA, int genB, int mother, int father);
	void PrintOutHare();
	void SimulateHare(int* food, int maxFood);
	bool IsAlive();
	bool IsHareMale();
	bool IsChild();
	bool IsReadyToProcreate();
	int GetHareFurFenotype();
	std::string GetHareFurFenotypeName();
	int hareID{0};
	int GetMoveVector();
	void HaveSex(Hare* partner);
	void WriteHareToDatabase(sqlite3* db);

	int ManagePregnacy();

	//TESTY - POTEM HERMETYZOWAC
	/*
	C = 3 jest w pe³ni dominuj¹cy wzglêdem innych;  allel "czarny"
	Cch = 2 jest niepe³nej dominacji wzglêdem himalajskiego i albinotycznego ; allel "szynszylowy"
	Ch = 1 himalajski allel
	c = 0  allelu albinotyczny
	*/
	int furGenotype[2] = { 0,0 };
	//kids father Data
	int fatherfurGenotype[2] = { 0,0 };
	int partnerID{ 0 };

	int hareFatherID{ 0 };
	int hareMotherID{ 0 };

	int age{ 0 };
	int food{ 450 };
private:
	
	bool isPregnant{ false };
	//150-200
	int foodUsage{ 150 };
	int maxFoodPerDay{ 300 };
	int maxFatSize{ 1500 };
	bool isMale {true};
	
	//28-35
	int pregnacyTime{ 31 };
	int pregnacyTimeLeft{ 0 };

	bool alreadyMoveThisDay{ false };

	int daySinceLastMate = 0;	
};

