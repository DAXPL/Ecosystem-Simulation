#include "HarePanel.h"

HarePanel::HarePanel()
{
	rectangle = new sf::RectangleShape(sf::Vector2f(width, height));
	rectangle->setFillColor(sf::Color::Cyan);
	rectangle->setPosition(0, 0);

	displayedText = new sf::Text();

	haresTex = nullptr;
	hareSprite = new sf::Sprite();
}

HarePanel::HarePanel(int x, int y, int panelWidth, int panelHeight, sf::Font* font, sf::Texture* hTex)
{
	width = panelWidth;
	height = panelHeight;
	rectangle = new sf::RectangleShape(sf::Vector2f(width, height));
	rectangle->setFillColor(sf::Color::Cyan);
	rectangle->setPosition(x, y);

	displayedText = new sf::Text();
	displayedText->setFont(*font);
	displayedText->setCharacterSize(16);
	displayedText->setFillColor(sf::Color());
	displayedText->setStyle(sf::Text::Bold);
	displayedText->setPosition(x+125, y);

	haresTex = hTex;
	hareSprite = new sf::Sprite();
	hareSprite->setTexture(*(hTex+2));
	hareSprite->setPosition(x, y);
	hareSprite->setScale(0.2f, 0.2f);
}

void HarePanel::DrawHareDesc(Hare* hare, sf::RenderWindow* window)
{
	if (hare == nullptr) 
	{
		std::cerr << "Nullptr to hare!" << std::endl;
		return;
	}
	hareSprite->setTexture(*(haresTex + (hare->IsChild() * 4) + hare->GetHareFurFenotype()));
	//std::string 
	displayedText->setString(
		  "Fenotyp: " + hare->GetHareFurFenotypeName() + '\n'
		+ "Wiek (dni):" + std::to_string(hare->age) + '\n'
		+ "Plec:" + std::to_string(hare->IsHareMale()) + '\n'
		+ "Kalorie:" + std::to_string(hare->food) + '\n');
	window->draw(*rectangle);
	window->draw(*hareSprite);
	window->draw(*displayedText);
}
