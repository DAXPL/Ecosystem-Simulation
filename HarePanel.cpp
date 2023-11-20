#include "HarePanel.h"

HarePanel::HarePanel()
{
	rectangle = new sf::RectangleShape(sf::Vector2f(width, height));
	rectangle->setFillColor(sf::Color::Cyan);
	rectangle->setPosition(0, 0);

	displayedText = new sf::Text();
}

HarePanel::HarePanel(int x, int y, int panelWidth, int panelHeight, sf::Font* font)
{
	width = panelWidth;
	height = panelHeight;
	rectangle = new sf::RectangleShape(sf::Vector2f(width, height));
	rectangle->setFillColor(sf::Color::Cyan);
	rectangle->setPosition(x, y);

	displayedText = new sf::Text();
	displayedText->setFont(*font);
	displayedText->setCharacterSize(14);
	displayedText->setFillColor(sf::Color::White);
	displayedText->setStyle(sf::Text::Bold);
	displayedText->setPosition(x, y);
}

void HarePanel::DrawHareDesc(Hare* hare, sf::RenderWindow* window)
{
	if (hare == nullptr) 
	{
		std::cerr << "Nullptr to hare!" << std::endl;
		return;
	}
	//std::string 
	displayedText->setString("Genotyp: "+std::to_string(hare->furGenotype[0])+" - " + std::to_string(hare->furGenotype[1]));
	window->draw(*rectangle);
	window->draw(*displayedText);
}
