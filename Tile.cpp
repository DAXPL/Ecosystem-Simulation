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

Tile::Tile(int foodAmout, sf::Font* font, int tileSize)
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

	std::cout << "tiles" << std::endl;
	std::cout << neighbors[0] << std::endl;
	std::cout << neighbors[1] << std::endl;
	std::cout << neighbors[2] << std::endl;
	std::cout << neighbors[3] << std::endl;
	std::cout << std::endl;
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

void Tile::SetTileNeighbors(Tile* left, Tile* up, Tile* right, Tile* down)
{
	neighbors[0] = left;
	neighbors[1] = up;
	neighbors[2] = right;
	neighbors[3] = down;
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

	for (int i = 0; i < hares.size(); i++)
	{
		if (hares.at(i) == nullptr)
		{
			std::cerr << "NULL HARE!" << std::endl;
			continue;
		}

		hares.at(i)->SimulateHare(&food, (food / hares.size()) * 1.1f);

	}

	// Usuwanie martwych zaj¹czków
	for (int i = 0; i < hares.size(); i++)
	{
		if (!hares.at(i)->IsAlive())
		{
			delete hares.at(i);
		}
	}

	// Wyczyszczenie listy zdech³ych zaj¹czków
	hares.erase(std::remove_if(hares.begin(), hares.end(), [](Hare* hare) { return !hare->IsAlive(); }), hares.end());

	if (food < maxFood) food += foodRegen;
}

void Tile::SimulateMove()
{
	std::vector<Hare*> haresStay;

	for (int i = 0; i < hares.size(); i++)
	{
		int moveVector = hares.at(i)->GetMoveVector();
		switch (moveVector)
		{
		case 1: case 2: case 3: case 4:
			if (neighbors[moveVector - 1] != nullptr && hares.at(i) != nullptr)
			{
				std::cerr << moveVector << std::endl;
				neighbors[moveVector - 1]->AddHare(hares.at(i));
			}
			else
			{
				std::cerr << "no neighbor" << std::endl;
				haresStay.push_back(hares.at(i));
			}
			break;
		default:
			haresStay.push_back(hares.at(i));
			break;
		}
	}

	// Usuñ zaj¹ce, które siê przenios³y
	hares.erase(std::remove_if(hares.begin(), hares.end(), [&haresStay](Hare* hare) {
		return std::find(haresStay.begin(), haresStay.end(), hare) != haresStay.end();
		}), hares.end());

	// Wyczyszczenie tymczasowej listy zywej
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
