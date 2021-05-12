#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game_main_screen.h"
#include "Game_menu_screen.h"

int main()
{
    sf::RenderWindow menu_window(sf::VideoMode(1200, 600), "Flip-card: Main Menu");
    //sf::RectangleShape rect(sf::Vector2f(2, 100));
    //window.clear(sf::Color::Green);
    menu_window.clear(sf::Color::White);
    while (menu_window.isOpen())
    {
        int code = gms::execute_menu(menu_window);
        std::cout << code << std::endl;
        switch (code)
        {
        case -1:
            break;
        default:
            menu_window.close();
            sf::RenderWindow window(sf::VideoMode(1860, 980), "Flip-card");
            window.clear(sf::Color::Green);
            while (window.isOpen())
            {
                gms::execute_game(window, code);
                window.close();
            }
            break;
            /*default:
                std::cerr << "Unexpected return code: " << code << std::endl;
                break;*/
        }
        menu_window.close();
    }

    return 0;
}