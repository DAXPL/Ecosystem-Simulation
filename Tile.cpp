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
		delete(hares.at(i));
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
		h->PrintOutHare();
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

void Tile::SimulateTile()
{
	std::vector<Hare*> haresAlive;
	std::vector<Hare*> deadHares;
	for (int i=0; i< hares.size();i++)
	{
		hares.at(i)->SimulateHare();
		if (hares.at(i)->IsAlive()) 
		{
			haresAlive.push_back(hares.at(i));
		}
		else 
		{
			deadHares.push_back(hares.at(i));
		}
	}
	hares.clear();

	for (int i = 0; i < haresAlive.size(); i++)
	{
		hares.push_back(haresAlive.at(i));
	}
	haresAlive.clear();

	for (int i = 0; i < deadHares.size(); i++)
	{
		delete deadHares.at(i);
	}
	deadHares.clear();
}
