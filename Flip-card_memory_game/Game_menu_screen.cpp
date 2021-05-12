#include <SFML/Graphics.hpp>
#include <iostream>

namespace gms
{

	int execute_menu(sf::RenderWindow& window)
	{
		sf::Event event;
		sf::Texture play_button_texture;
		sf::Texture exit_button_texture;
		sf::Texture easy_button_texture_raw;
		sf::Texture meduim_button_texture_raw;
		sf::Texture hard_button_texture_raw;
		sf::Texture easy_button_texture_pressed;
		sf::Texture meduim_button_texture_pressed;
		sf::Texture hard_button_texture_pressed;
		play_button_texture.loadFromFile("play_button.png");
		exit_button_texture.loadFromFile("exit_button.png");
		easy_button_texture_raw.loadFromFile("easy.png");
		meduim_button_texture_raw.loadFromFile("meduim.png");
		hard_button_texture_raw.loadFromFile("hard.png");
		easy_button_texture_pressed.loadFromFile("easy_pressed.png");
		meduim_button_texture_pressed.loadFromFile("medium_pressed.png");
		hard_button_texture_pressed.loadFromFile("hard_pressed.png");
		sf::Sprite play_button_sprite(play_button_texture);
		sf::Sprite exit_button_sprite(exit_button_texture);
		sf::Sprite easy_button_sprite(easy_button_texture_raw);
		sf::Sprite meduim_button_sprite(meduim_button_texture_raw);
		sf::Sprite hard_button_sprite(hard_button_texture_raw);
		bool EasyPressed = false;
		bool MediumPressed = false;
		bool HardPressed = false;

		while (window.isOpen())
		{
			window.clear(sf::Color::White);
			play_button_sprite.setPosition(368, 150);
			exit_button_sprite.setPosition(368, 300);
			easy_button_sprite.setPosition(310, 20);
			meduim_button_sprite.setPosition(510, 20);
			hard_button_sprite.setPosition(710, 20);
			window.draw(play_button_sprite);
			window.draw(exit_button_sprite);
			window.draw(easy_button_sprite);
			window.draw(meduim_button_sprite);
			window.draw(hard_button_sprite);
			window.display();

			if (EasyPressed)
				easy_button_sprite.setTexture(easy_button_texture_pressed);
			else
				easy_button_sprite.setTexture(easy_button_texture_raw);

			if (MediumPressed)
				meduim_button_sprite.setTexture(meduim_button_texture_pressed);
			else
				meduim_button_sprite.setTexture(meduim_button_texture_raw);

			if (HardPressed)
				hard_button_sprite.setTexture(hard_button_texture_pressed);
			else
				hard_button_sprite.setTexture(hard_button_texture_raw);

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					return -1;
				if (event.type == sf::Event::MouseButtonPressed)
				{
					if (event.key.code == sf::Mouse::Left)
					{
						sf::Vector2i pos = sf::Mouse::getPosition(window);
						if ((368 <= pos.x) && (pos.x <= 832) && (150 <= pos.y) && (pos.y <= 289)) {
							if (EasyPressed)
								return 1;
							if (MediumPressed)
								return 2;
							if (HardPressed)
								return 3;
						}
						if ((368 <= pos.x) && (pos.x <= 832) && (300 <= pos.y) && (pos.y <= 439))
							return -1;
						if ((310 <= pos.x) && (pos.x <= 490) && (20 <= pos.y) && (pos.y <= 110))
						{
							MediumPressed = false;
							HardPressed = false;
							if (EasyPressed)
								EasyPressed = false;
							else
								EasyPressed = true;
						}
						if ((510 <= pos.x) && (pos.x <= 690) && (20 <= pos.y) && (pos.y <= 110))
						{
							EasyPressed = false;
							HardPressed = false;
							if (MediumPressed)
								MediumPressed = false;
							else
								MediumPressed = true;
						}
						if ((710 <= pos.x) && (pos.x <= 990) && (20 <= pos.y) && (pos.y <= 110)) {
							MediumPressed = false;
							EasyPressed = false;
							if (HardPressed)
								HardPressed = false;
							else
								HardPressed = true;
						}
					}
				}
			}
		}
	}

}	// namespace gms