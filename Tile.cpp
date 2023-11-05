#include "Tile.h"

Tile::Tile()
{
	food = 25;
}

Tile::Tile(int foodAmout)
{
	food = foodAmout;
}

Tile::~Tile()
{
	for(int i= (hares.size() -1);i>=0;i--)
	{
		Hare* h = hares.at(i);
		delete(h);
	}
	hares.clear();
}

void Tile::AddHare(Hare* newHare)
{
	hares.push_back(newHare);
}

void Tile::PrintOutHares()
{
	std::cout << "Liczba zajecy: "<< hares.size() << std::endl;
	for (Hare* h : hares) 
	{
		std::cout << h << std::endl;
	}
	std::cout << std::endl;
}
