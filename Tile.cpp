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
	if(newHare != nullptr) hares.push_back(newHare);
}

void Tile::PrintOutHares()
{
	std::cout << "Liczba zajecy w "<<this<<": "<< hares.size() << std::endl;
	for (Hare* h : hares) 
	{
		if (h != nullptr)h->PrintOutHare();
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
	displayedText->setString(std::to_string(hares.size())+'\n'+std::to_string(food));
	window->draw(*rectangle);
	window->draw(*displayedText);
}

void Tile::SimulateTile(sqlite3* db)
{
	std::vector<Hare*> maleHaresToProcreate;
	std::vector<Hare*> femaleHaresToProcreate;

	for (int i = 0; i < hares.size(); i++)
	{
		if (hares.at(i) == nullptr)
		{
			continue;
		}

		hares.at(i)->SimulateHare(&food, (food / hares.size()) * 1.1f);
		if (hares.at(i)->IsReadyToProcreate()) 
		{
			if (hares.at(i)->IsHareMale()) maleHaresToProcreate.push_back(hares.at(i));
			else femaleHaresToProcreate.push_back(hares.at(i));
		}

		int litters = hares.at(i)->ManagePregnacy();
		if (litters > 0)
		{
			int motherGenotype[2] = { hares.at(i)->furGenotype[0],hares.at(i)->furGenotype[1]};
			int FatherGenotype[2] = { hares.at(i)->fatherfurGenotype[0],hares.at(i)->fatherfurGenotype[1] };
			int chess[4][2] =
			{
				{motherGenotype[0],FatherGenotype[0]},
				{motherGenotype[1],FatherGenotype[0]},
				{motherGenotype[0],FatherGenotype[1]},
				{motherGenotype[1],FatherGenotype[1]}
			};

			for (int h = 0; h < litters; h++) 
			{
				int type = (int)(rand() % 4);
				AddHare(new Hare(chess[type][0], chess[type][1], hares.at(i)->hareID, hares.at(i)->partnerID));
			}
		}
	}

	for (int i = 0; i < maleHaresToProcreate.size() && i < femaleHaresToProcreate.size(); i++) 
	{
		maleHaresToProcreate.at(i)->HaveSex(femaleHaresToProcreate.at(i));
		femaleHaresToProcreate.at(i)->HaveSex(maleHaresToProcreate.at(i));
	}

	// Usuwanie martwych zaj¹czków
	for (int i = 0; i < hares.size(); i++)
	{
		if (!hares.at(i)->IsAlive())
		{
			hares.at(i)->WriteHareToDatabase(db);

			
			delete hares.at(i);
		}
	}

	// Wyczyszczenie listy zdech³ych zaj¹czków
	hares.erase(std::remove_if(hares.begin(), hares.end(), [](Hare* hare) { return !hare->IsAlive(); }), hares.end());

	if (food < maxFood) food += foodRegen;
}

void Tile::SimulateMove()
{
	int a = int(this);
	int b = int(neighbors[0]);
	auto i = hares.begin();
	while(i != hares.end())
	{
		if (*i == nullptr) 
		{
			i++;
			continue;
		}
		
		int moveVector = (*i)->GetMoveVector();

		if (moveVector >= 1 && moveVector <= 4 && neighbors[moveVector - 1] != nullptr)
		{
			neighbors[moveVector - 1]->AddHare(*i);
			hares.erase(i);
		}
		else
		{
			i++;
		}
	}
}

bool Tile::IsClicked(int x, int y)
{
	int recX= rectangle->getPosition().x;
	int recY= rectangle->getPosition().y;
	int sizeX= rectangle->getGlobalBounds().getSize().x;
	int sizeY= rectangle->getGlobalBounds().getSize().y;

	return (x >= recX && x <= (recX + sizeX)) && (y >= recY && y <= (recY + sizeY));
}

Hare* Tile::GetHare(int id)
{
	if (id < 0 || id >= hares.size()) return nullptr;
	return hares.at(id);
}

int Tile::GetHaresCount()
{
	return hares.size();
}
