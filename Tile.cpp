#include "Tile.h"
//prze³adowaæ konstruktor
Tile::Tile()
{
	food = 25;
	rectangle = new sf::RectangleShape(sf::Vector2f(50.0f, 50.0f));
	rectangle->setFillColor(sf::Color::Red);
}

Tile::Tile(int foodAmout)
{
	food = foodAmout;
	rectangle = new sf::RectangleShape(sf::Vector2f(50.0f, 50.0f));
	rectangle->setFillColor(sf::Color::Red);
}

Tile::~Tile()
{
	for(int i= (hares.size() -1);i>=0;i--)
	{
		Hare* h = hares.at(i);
		delete(h);
	}
	hares.clear();
	delete(rectangle);
}

void Tile::AddHare(Hare* newHare)
{
	hares.push_back(newHare);
}

void Tile::PrintOutHares()
{
	std::cout << "Liczba zajecy w "<<this<<": "<< hares.size() << std::endl;
	for (Hare* h : hares) 
	{
		std::cout << h << std::endl;
	}
	std::cout << std::endl;
}

void Tile::SetPosition(int x, int y)
{
	rectangle->setPosition(x, y);
	std::cout << x << " | " << y << std::endl;
}

void Tile::DrawTile(sf::RenderWindow* window)
{
	window->draw(*rectangle);
}
