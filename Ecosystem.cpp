#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

sf::CircleShape shape(300.f);

void Draw(sf::RenderWindow* window);

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }

        Draw(&window);
    }

    return 0;
}

void Draw(sf::RenderWindow* window)
{
    window->clear();
    window->draw(shape);
    window->display();
}
