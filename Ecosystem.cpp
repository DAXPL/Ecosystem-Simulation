#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Hare.h"
#include "Tile.h"
#include "HarePanel.h"

const int TILES_SIZE{5};

const int HARE_PANELS{ 5 };

const int TILES_GFX_SIZE{ 50 };
const int TILES_MARGIN{ 10 };

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
    dayCounterText.setFillColor(sf::Color(161, 254, 161));
    dayCounterText.setStyle(sf::Text::Bold);
    dayCounterText.setPosition(460,5);
    dayCounterText.setString("Simulation day: " + std::to_string(0));

    int startY = 75;
    int marginY = 5;
    int panelHeight =  ((window.getSize().y-startY) / HARE_PANELS) - marginY;
    HarePanel* descPanels[HARE_PANELS];
    sf::Texture haresTexes[2][4] = {
        {sf::Texture(),sf::Texture(),sf::Texture(),sf::Texture()},
        {sf::Texture(),sf::Texture(),sf::Texture(),sf::Texture()}
    };
    for (int i = 0; i < 4; i++)
    {
        haresTexes[0][i].loadFromFile("Kicajec" + std::to_string(i) + ".png");
        haresTexes[1][i].loadFromFile("BabyKicajec" + std::to_string(i) + ".png");
    }
    for (int i = 0; i < HARE_PANELS; i++)
    {
        descPanels[i] = new HarePanel(450, (startY+((100+marginY)*i)),300, panelHeight, mainFont, *haresTexes);
    }

    //Generate terrain
    Tile* tiles[TILES_SIZE][TILES_SIZE];
    
    for (int i = 0; i < TILES_SIZE; i++)
    {
        for (int j = 0; j < TILES_SIZE; j++)
        {
            tiles[i][j] = new Tile((rand() % 1500)+1000,mainFont,TILES_GFX_SIZE);
            int howManyHares = rand() % 10;
            for (int z = 0; z < howManyHares; z++)
            {
                tiles[i][j]->AddHare(new Hare());
            }
        }
    }

    for (int i = 0; i < TILES_SIZE; i++)
    {
        for (int j = 0; j < TILES_SIZE; j++)
        {
            tiles[i][j]->SetPosition(((TILES_GFX_SIZE + TILES_MARGIN) * i), ((TILES_GFX_SIZE + TILES_MARGIN) * j));
            tiles[i][j]->SetTileNeighbors(
                tiles[(i<=0)?(TILES_SIZE -1):(i-1)]             [j],//left
                tiles[i]                                        [(j<=0)?(TILES_SIZE-1):(j+1)],//up
                tiles[(i >= (TILES_SIZE -1)) ? (0) : (i + 1)]   [j],//right
                tiles[i]                                        [(j>=TILES_SIZE-1)?(0):(j-1)]//down
                );
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
                            for (int i = 0; i < TILES_SIZE; i++)
                            {
                                for (int j = 0; j < TILES_SIZE; j++)
                                {
                                    tiles[i][j]->SimulateTile();
                                }
                            }

                            for (int i = 0; i < TILES_SIZE; i++)
                            {
                                for (int j = 0; j < TILES_SIZE; j++)
                                {
                                    tiles[i][j]->SimulateMove();
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

                            for (int i = 0; i < TILES_SIZE; i++)
                            {
                                for (int j = 0; j < TILES_SIZE; j++)
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
                    if (selectedTile != nullptr) 
                    {
                        selectedOffset += event.mouseWheel.delta;
                        int maxOffset = selectedTile->GetHaresCount() - HARE_PANELS;
                        if (maxOffset < 0) maxOffset = 0;
                        if (selectedOffset < 0) selectedOffset = 0;
                        if (selectedOffset > maxOffset)selectedOffset = maxOffset;
                    }
                    break;
            }
        }
        if (!runSimulation) continue; 

        window.clear();
        window.draw(bg);

        for (int i = 0; i < TILES_SIZE; i++)
        {
            for (int j = 0; j < TILES_SIZE; j++)
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
    //wyczyscic pamiec
    std::cout << "End" << std::endl;
    return 0;
}