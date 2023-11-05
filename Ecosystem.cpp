#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Hare.h"
#include "Tile.h"
sf::CircleShape shape(300.f);

void Draw(sf::RenderWindow* window);

int main()
{
    Tile* t = new Tile(75);
    
    sf::RenderWindow window(sf::VideoMode(800, 600), "Simulation");
    shape.setFillColor(sf::Color::Green);

    for (int i = 0; i < 10; i++) 
    {
        t->AddHare(new Hare());
    }
    t->PrintOutHares();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }

        Draw(&window);
    }

    delete(t);
    return 0;
}

void Draw(sf::RenderWindow* window)
{
    window->clear();
    window->draw(shape);
    window->display();
}
