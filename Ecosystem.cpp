#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Hare.h"
#include "Tile.h"

const int TILES_ROWS{5};
const int TILES_COLUMS{5};

const int TILES_SIZE{ 50 };
const int TILES_MARGIN{ 10 };

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Simulation");
    sf::Font* mainFont = new sf::Font();
    if (!mainFont->loadFromFile("Bodoni.ttf"))
    {
        std::cerr << "Cant load font!" << std::endl;
        return -1;
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
                            std::cout << "Day:" << std::to_string(day) << std::endl;
                            for (int i = 0; i < TILES_ROWS; i++)
                            {
                                for (int j = 0; j < TILES_COLUMS; j++)
                                {
                                    tiles[i][j]->SimulateTile();
                                }
                            }
                            day++;
                            std::cout << std::endl;
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
                                        tiles[i][j]->PrintOutHares();
                                    }
                                }
                            }

                        break;
                    }
                    break;
            }
        }
        if (!runSimulation) continue; 

        window.clear();

        for (int i = 0; i < TILES_ROWS; i++)
        {
            for (int j = 0; j < TILES_COLUMS; j++)
            {
                tiles[i][j]->DrawTile(&window);
            }
        }

        window.display();
        std::cin;
    }

    std::cout << "Ending simulation" << std::endl;
    window.close();
    for (int i=0; i<TILES_ROWS;i++) 
    {
        delete[] tiles[i];
    }

    delete[] tiles;
    std::cout << "End" << std::endl;
    return 0;
}