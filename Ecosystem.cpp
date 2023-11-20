#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Hare.h"
#include "Tile.h"
#include "HarePanel.h"

const int TILES_ROWS{5};
const int TILES_COLUMS{5};

const int HARE_PANELS{ 5 };

const int TILES_SIZE{ 50 };
const int TILES_MARGIN{ 10 };

static int haresId{0};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Simulation");

    //GENERATE UI
    sf::Font* mainFont = new sf::Font();
    if (!mainFont->loadFromFile("Bodoni.ttf"))
    {
        std::cerr << "Cant load font!" << std::endl;
        return -1;
    }

    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("Background.jpg"))
    {
        std::cerr << "Cant load Background.jpg!" << std::endl;
        return -1;
    }

    sf::Sprite bg(bgTexture);
    float scaleX = (float)window.getSize().x / (float)bgTexture.getSize().x;
    float scaleY = (float)window.getSize().y / (float)bgTexture.getSize().y;
    bg.setScale(scaleX, scaleY);

    sf::Text dayCounterText;
    dayCounterText.setFont(*mainFont);
    dayCounterText.setCharacterSize(40);
    dayCounterText.setFillColor(sf::Color::White);
    dayCounterText.setStyle(sf::Text::Bold);
    dayCounterText.setPosition(460,5);
    dayCounterText.setString("Simulation day: " + std::to_string(0));

    int startY = 75;
    int marginY = 5;
    int panelHeight =  ((window.getSize().y-startY) / HARE_PANELS) - marginY;
    HarePanel* descPanels[HARE_PANELS];
    for (int i = 0; i < HARE_PANELS; i++)
    {
        descPanels[i] = new HarePanel(450, (startY+((100+marginY)*i)),300, panelHeight, mainFont);
    }

    //Generate terrain
    Tile* tiles[TILES_ROWS][TILES_COLUMS];
    
    for (int i = 0; i < TILES_ROWS; i++)
    {
        for (int j = 0; j < TILES_COLUMS; j++)
        {
            tiles[i][j] = new Tile(75, mainFont, TILES_SIZE);
            int howManyHares = rand() % 10;
            for (int z = 0; z < howManyHares; z++)
            {
                tiles[i][j]->AddHare(new Hare());
            }
        }
    }

    for (int i = 0; i < TILES_ROWS; i++)
    {
        for (int j = 0; j < TILES_COLUMS; j++)
        {
            tiles[i][j]->SetPosition(((TILES_SIZE+ TILES_MARGIN) * i), ((TILES_SIZE + TILES_MARGIN) * j));
        }
    }

    //Simulation
    bool runSimulation = true;
    int day{ 0 };
    Tile* selectedTile{ nullptr };
    int selectedOffset{ 0 };
    while (runSimulation)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    runSimulation = false;
                    break;
                case sf::Event::KeyPressed:
                    switch (event.key.code) 
                    {
                        case sf::Keyboard::Space:
                            day++;
                            std::cout << "Day:" << std::to_string(day) << std::endl;
                            dayCounterText.setString("Simulation day:" + std::to_string(day));
                            for (int i = 0; i < TILES_ROWS; i++)
                            {
                                for (int j = 0; j < TILES_COLUMS; j++)
                                {
                                    tiles[i][j]->SimulateTile();
                                }
                            }
                        break;
                    }
                    break;
                case sf::Event::MouseButtonPressed:
                    switch (event.key.code) 
                    {
                        case sf::Mouse::Left:
                            int xPos = sf::Mouse::getPosition(window).x;
                            int yPos = sf::Mouse::getPosition(window).y;

                            for (int i = 0; i < TILES_ROWS; i++)
                            {
                                for (int j = 0; j < TILES_COLUMS; j++)
                                {
                                    if (tiles[i][j]->IsClicked(xPos, yPos)) 
                                    {
                                        selectedTile = tiles[i][j];
                                        selectedOffset = 0;
                                    }
                                }
                            }

                        break;
                    }
                    break;
                case sf::Event::MouseWheelMoved:
                    selectedOffset += event.mouseWheel.delta;
                    if (selectedOffset < 0) selectedOffset = 0;
                    if (selectedOffset > selectedTile->GetHaresCount())selectedOffset = selectedTile->GetHaresCount();
                    break;
            }
        }
        if (!runSimulation) continue; 

        window.clear();
        window.draw(bg);

        for (int i = 0; i < TILES_ROWS; i++)
        {
            for (int j = 0; j < TILES_COLUMS; j++)
            {
                tiles[i][j]->DrawTile(&window);
            }
        }

        if (selectedTile != nullptr) 
        {
            int hareCount = selectedTile->GetHaresCount();
           
            for (int i = 0; i < HARE_PANELS; i++)
            {
                int harePointer = selectedOffset + i;

                if (harePointer < hareCount) 
                {
                    Hare* h = selectedTile->GetHare(harePointer);

                    if (h != nullptr)
                    {
                        descPanels[i]->DrawHareDesc(h, &window);
                    }
                    else 
                    {
                        break;
                    }
                }
            }
        }
        
        window.draw(dayCounterText);
        window.display();
    }

    std::cout << "Ending simulation" << std::endl;
    window.close();

    for (int i=0; i<TILES_ROWS;i++) 
    {
        delete[] tiles[i];
    }

    delete[] tiles;
    delete[] descPanels;
    std::cout << "End" << std::endl;
    return 0;
}