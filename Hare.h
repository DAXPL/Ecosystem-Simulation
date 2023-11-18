#pragma once
class Hare
{
public:
	Hare();
	void PrintOutHare();
	void SimulateHare();
	bool IsAlive();
	bool IsHareMale();
	int hareID{0};

	//TESTY - POTEM HERMETYZOWAC
	/*
	C = 3 jest w pe³ni dominuj¹cy wzglêdem innych;  allel "czarny"
	Cch = 2 jest niepe³nej dominacji wzglêdem himalajskiego i albinotycznego ; allel "szynszylowy"
	Ch = 1 himalajski allel
	c = 0  allelu albinotyczny
	*/
	int furGenotype[2] = { 0,0 };
private:
	int food {25};
	bool isMale {true};
	
};

