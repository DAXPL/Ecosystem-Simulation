#pragma once
class Hare
{
public:
	Hare();
	void PrintOutHare();
	void SimulateHare();
	bool IsAlive();
	bool IsHareMale();
private:
	int food {25};
	bool isMale {true};

};

