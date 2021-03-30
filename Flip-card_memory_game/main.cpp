#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game_main_screen.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 600), "Flip-card");
    sf::RectangleShape rect(sf::Vector2f(2, 100));
    window.clear(sf::Color::Green);
    while (window.isOpen())
    {
        gms::execute_game(window);
        window.close();
    }

    return 0;
}