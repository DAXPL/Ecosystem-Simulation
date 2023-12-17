#include "Tile.h"
//prze³adowaæ konstruktor
Tile::Tile()
{
	food = 2000;
	rectangle = new sf::RectangleShape(sf::Vector2f(25.0f, 25.0f));
	rectangle->setFillColor(sf::Color::Red);
	displayedText = new sf::Text();
	displayedText->setCharacterSize(25);
	displayedText->setFillColor(sf::Color::Green);
}

Tile::Tile(int foodAmout, sf::Font* font, Tile* left, Tile* up, Tile* right, Tile* down, int tileSize)
{
	food = foodAmout;
	foodRegen = foodAmout / 4;
	rectangle = new sf::RectangleShape(sf::Vector2f(tileSize, tileSize));
	rectangle->setFillColor(sf::Color::Cyan);

	displayedText = new sf::Text();
	displayedText->setFont(*font);
	displayedText->setCharacterSize(24);
	displayedText->setFillColor(sf::Color::White);
	displayedText->setStyle(sf::Text::Bold);

	neighbors[0] = left;
	neighbors[1] = up;
	neighbors[2] = right;
	neighbors[3] = down;
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
	displayedText->setString(std::to_string(haresCount)+'\n'+std::to_string(food));
	window->draw(*rectangle);
	window->draw(*displayedText);
}

void Tile::SimulateTile()
{
	std::vector<Hare*> haresAlive;
	std::vector<Hare*> deadHares;

	for (int i=0; i< hares.size();i++)
	{
		hares.at(i)->SimulateHare(&food,(food/ hares.size())*1.1f);

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

	if(food<maxFood) food += foodRegen;
}

void Tile::SimulateMove()
{
	std::vector<Hare*> haresStay;

	for (int i = 0; i < hares.size(); i++)
	{
		int moveVector = hares.at(i)->GetMoveVector();
		switch (moveVector) 
		{
			case 0:
				haresStay.push_back(hares.at(i));
				break;
			case 1:
				if (neighbors[0] != nullptr) 
				{
					neighbors[0]->AddHare(hares.at(i));
				} 
				else 
				{
					std::cerr << "no neighbor at left!" << std::endl;
					haresStay.push_back(hares.at(i));
				} 
				break;
			case 2:
				if (neighbors[1] != nullptr)
				{
					neighbors[1]->AddHare(hares.at(i));
				}
				else
				{
					std::cerr << "no neighbor up!" << std::endl;
					haresStay.push_back(hares.at(i));
				}
				break;
			case 3:
				if (neighbors[2] != nullptr)
				{
					neighbors[2]->AddHare(hares.at(i));
				}
				else
				{
					std::cerr << "no neighbor at right!" << std::endl;
					haresStay.push_back(hares.at(i));
				} 
				break;
			case 4:
				if (neighbors[3] != nullptr)
				{
					neighbors[3]->AddHare(hares.at(i));
				}
				else
				{
					std::cerr << "no neighbor down!" << std::endl;
					haresStay.push_back(hares.at(i));
				} 
				break;
		}
	}
	hares.clear();

	for (int i = 0; i < haresStay.size(); i++)
	{
		hares.push_back(haresStay.at(i));
	}
	haresStay.clear();
}

bool Tile::IsClicked(int x, int y)
{
	int recX = rectangle->getPosition().x;
	int recY = rectangle->getPosition().y;
	int sizeX = rectangle->getGlobalBounds().getSize().x;
	int sizeY = rectangle->getGlobalBounds().getSize().y;

	return (x >= recX && x <= (recX + sizeX)) && (y >= recY && y <= (recY + sizeY));
}

Hare* Tile::GetHare(int id)
{
	if (id < 0 || id >= hares.size()) 
	{
		//std::cerr << "id outside of boundaries!" << std::endl;
		return nullptr;
	}
	return hares.at(id);
}

int Tile::GetHaresCount()
{
	return hares.size();
}
