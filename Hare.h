#pragma once
class Hare
{
public:
	Hare();
	void PrintOutHare();
	void SimulateHare();
	bool IsAlive();
	bool IsHareMale();
	bool IsChild();
	int GetHareFurFenotype();
	int hareID{0};
	void HaveSex(Hare* partner);

	//TESTY - POTEM HERMETYZOWAC
	/*
	C = 3 jest w pe³ni dominuj¹cy wzglêdem innych;  allel "czarny"
	Cch = 2 jest niepe³nej dominacji wzglêdem himalajskiego i albinotycznego ; allel "szynszylowy"
	Ch = 1 himalajski allel
	c = 0  allelu albinotyczny
	*/
	int furGenotype[2] = { 0,0 };
	int age{ 0 };
	int food{ 450 };
	
private:
	
	bool isPregnant{ false };
	//150-200
	int foodUsage{ 150 };
	bool isMale {true};
	
	//28-35
	int pregnacyTIme{ 31 };
	int pregnacyTimeLeft{ 0 };
	
};

