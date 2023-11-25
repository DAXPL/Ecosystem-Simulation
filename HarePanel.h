#pragma once
#include "Hare.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

class HarePanel
{
private:
	sf::RectangleShape* rectangle;
	sf::Text* displayedText;
	sf::Sprite* hareSprite;
	int xPos{ 0 };
	int yPos{ 0 };
	int width{ 300 };
	int height{ 100 };
	sf::Texture* haresTex;
public:
	HarePanel();
	HarePanel(int x, int y, int panelWidth, int panelHeight, sf::Font* font, sf::Texture* hTex);
	void DrawHareDesc (Hare* hare, sf::RenderWindow* window);
};

