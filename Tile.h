#pragma once
#include <vector>
#include "Hare.h"
#include <iostream>
class Tile
{
private:
	int food{25};
	std::vector<Hare*> hares;
public:
	Tile();
	Tile(int foodAmout);
	~Tile();
	void AddHare(Hare* newHare);
	void PrintOutHares();
};

