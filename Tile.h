#pragma once
#include <vector>
#include "Hare.h"
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <sqlite3.h>

class Tile
{
private:
	int food{2000};
	int foodRegen{750};
	int maxFood{ 4000 };

	Tile* neighbors[4]{nullptr,nullptr,nullptr,nullptr};//left, up, right, down

	std::vector<Hare*> hares;
	sf::RectangleShape* rectangle;
	sf::Text* displayedText;
public:
	Tile();
	Tile(int foodAmout, sf::Font* font, int tileSize = 25);
	~Tile();
	void SetTileNeighbors( Tile* left, Tile* up, Tile* right, Tile* down);
	void AddHare(Hare* newHare);
	void PrintOutHares();
	void SetPosition(int x, int y);
	void DrawTile(sf::RenderWindow* window);
	void SimulateTile(sqlite3* db);
	void SimulateMove();
	bool IsClicked(int x, int y);
	Hare* GetHare(int id);
	int GetHaresCount();
};

