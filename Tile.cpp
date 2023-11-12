#include "Tile.h"
//prze³adowaæ konstruktor
Tile::Tile()
{
	food = 25;
	rectangle = new sf::RectangleShape(sf::Vector2f(25.0f, 25.0f));
	rectangle->setFillColor(sf::Color::Red);
	displayedText = new sf::Text();
	displayedText->setCharacterSize(25);
	displayedText->setFillColor(sf::Color::Green);
}

Tile::Tile(int foodAmout, sf::Font* font, int tileSize)
{
	food = foodAmout;
	rectangle = new sf::RectangleShape(sf::Vector2f(tileSize, tileSize));
	rectangle->setFillColor(sf::Color::Red);

	displayedText = new sf::Text();
	displayedText->setFont(*font);
	displayedText->setCharacterSize(24);
	displayedText->setFillColor(sf::Color::Green);
	displayedText->setStyle(sf::Text::Bold | sf::Text::Underlined);
}

Tile::~Tile()
{
	for(int i= (hares.size() -1);i>=0;i--)
	{
		Hare* h = hares.at(i);
		delete(h);
	}
	hares.clear();
	delete(displayedText);
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
	displayedText->setPosition(x, y);
}

void Tile::DrawTile(sf::RenderWindow* window)
{
	int haresCount = hares.size();
	displayedText->setString(std::to_string(haresCount));
	window->draw(*rectangle);
	window->draw(*displayedText);
}
