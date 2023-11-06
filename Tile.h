#pragma once
#include <vector>
#include "Hare.h"
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
class Tile
{
private:
	int food{25};
	std::vector<Hare*> hares;
	sf::RectangleShape* rectangle;
public:
	Tile();
	Tile(int foodAmout);
	~Tile();
	void AddHare(Hare* newHare);
	void PrintOutHares();
	void SetPosition(int x, int y);
	void DrawTile(sf::RenderWindow* window);
};

