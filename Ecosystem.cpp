#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Hare.h"
#include "Tile.h"

const int TILES_ROWS{3};
const int TILES_COLUMS{3};
const int TILES_SIZE{ 10 };
const int TILES_MARGIN{ 0 };

int main()
{
    Tile* tiles[TILES_ROWS][TILES_COLUMS] =
    {
        {new Tile(75),new Tile(75),new Tile(75)},
        {new Tile(75),new Tile(75),new Tile(75)},
        {new Tile(75),new Tile(75),new Tile(75)}
    };
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Simulation");

    for (int i = 0; i < 10; i++) 
    {
        tiles[0][0]->AddHare(new Hare());
    }
    tiles[0][0]->PrintOutHares();

    for (int i = 0; i < TILES_ROWS; i++)
    {
        for (int j = 0; j < TILES_COLUMS; j++)
        {
            tiles[i][j]->SetPosition((TILES_SIZE * i) + TILES_MARGIN, (TILES_SIZE * j) + TILES_MARGIN);
        }
    }


    bool runSimulation = true;
    while (runSimulation)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) runSimulation = false; 
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
    }

    window.close();
    for (int i=0; i<TILES_ROWS;i++) 
    {
        delete[] tiles[i];
    }

    delete[] tiles;
    return 0;
}