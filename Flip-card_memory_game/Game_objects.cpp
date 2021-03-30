#include <SFML/Graphics.hpp>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "Game_objects.h"

const int standart_x = 200;
const int standart_y = 100;

namespace gobj
{
	int get_id_from_name(std::string);

	Table::Table(const size_t w, const size_t h, const size_t fr_w, sf::Sprite& s, const sf::Vector2i& start_pos)
		: width(w), height(h), frame_w(fr_w), sprite(s), draw_pos(start_pos)
	{
		if (frame_w * 2 >= std::min(width, height))
		{
			frame_w = std::max(width, height) / 2;
		}
	}

	Table::Table(sf::RenderWindow& win, const size_t s_w, const size_t s_h, sf::Sprite& s, const sf::Vector2i& start_pos)
		: frame_w(1), sprite(s), draw_pos(start_pos)
	{
		width = s_w / 6 - 1;
		height = s_h / 6 - 1;
	}

	Table::Table()
		:	sprite(*(new sf::Sprite)), draw_pos(sf::Vector2i(0, 0)), width(0), height(0), frame_w(0)
	{}

	void Table::draw(sf::RenderWindow& window) const
	{
		sprite.setScale(sf::Vector2f(static_cast<float>(width - 2 * frame_w) / standart_x, static_cast<float>(height - 2 * frame_w) / standart_y));
		sprite.setPosition(sf::Vector2f(draw_pos.x + frame_w, draw_pos.y + frame_w));
		window.draw(sprite);
	}

	Game_button::Game_button(const size_t w, const size_t h, const size_t fr_w, sf::Sprite& s, sf::Sprite& sec_s, sf::Sprite& back, const sf::Vector2i& start_pos, const int new_id)
		: Table(w, h, fr_w, s, start_pos), second_sprite(sec_s), background(back),
		back_draw_pos(start_pos), back_width(w), id(new_id)
	{}

	Game_button::Game_button(sf::RenderWindow& win, const size_t s_w, const size_t s_h, sf::Sprite& s, sf::Sprite& sec_s, sf::Sprite& back, const sf::Vector2i& start_pos, const int new_id)
		: Table(win, s_w, s_h, s, start_pos), second_sprite(sec_s), background(back), back_draw_pos(start_pos), id(new_id)
	{
		back_width = s_w / 6 - 1;
	}

	Game_button::Game_button(Game_button&& new_r_button)
		: Table(new_r_button.width, new_r_button.height, new_r_button.frame_w, 
		new_r_button.sprite, new_r_button.draw_pos), second_sprite(new_r_button.second_sprite),
		background(new_r_button.background), back_draw_pos(new_r_button.back_draw_pos),
		back_width(new_r_button.back_width)
	{}

	Game_button::Game_button()
		: Table(), background(*(new sf::Sprite)), second_sprite(*(new sf::Sprite)), back_draw_pos(0, 0), back_width(0), id(0)
	{}

	bool Game_button::is_mouse_this(sf::RenderWindow& window) const
	{
		sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
		if (mouse_pos.x > (draw_pos.x + frame_w) && mouse_pos.x < (draw_pos.x + width - frame_w) && mouse_pos.y >(draw_pos.y + frame_w) && mouse_pos.y < (draw_pos.y + height - frame_w))
		{
			return true;
		}
		return false;
	}

	void Game_button::draw(sf::RenderWindow& window) const
	{
		if (is_mouse_this(window))
		{
			sprite.setColor(sf::Color(128, 128, 128));
		}
		else
		{
			sprite.setColor(sf::Color::White);
		}
		Table::draw(window);
	}

	void Game_button::draw_background(sf::RenderWindow& window) const
	{
		background.setScale(sf::Vector2f(float(back_width) / standart_x, float(height) / standart_y));
		background.setPosition(sf::Vector2f(back_draw_pos.x, back_draw_pos.y));
		//std::cout << x0 << "\t" << y0 << std::endl;
		window.draw(background);
	}

	void Game_button::turn_over(sf::RenderWindow& window)
	{
		//sprite.setColor(sf::Color::Blue);

		int speed = 1;

		std::cout << "it works" << std::endl;
		sf::Clock clock;
		float delay = 0.001;
		float timer = 0;
		while (width > 2 * frame_w)
		{
			clock.restart();
			timer += clock.getElapsedTime().asSeconds();
			if (timer > delay)
			{
				draw_pos.x += speed;
				width -= 2 * speed;
				draw_background(window);
				draw(window);
				window.display();
				timer = 0;
			}
		}
		std::swap(sprite, second_sprite);
		//std::cout << "Background width: " << back_width << std::endl;
		while (width < back_width)
		{
			clock.restart();
			timer += clock.getElapsedTime().asSeconds();
			if (timer > delay)
			{
				draw_pos.x -= speed;
				width += 2 * speed;
				draw_background(window);
				draw(window);
				window.display();
				timer = 0;
			}
		}
		draw_pos.x = back_draw_pos.x;
		draw_pos.y = back_draw_pos.y;

	}

	void Game_button::turn_over_dx(Flags_for_turn f)
	{
		int speed = 1;
		//std::cout << "Lable in turn_over" << std::endl;
		switch (f)
		{
		case gobj::TURN_TO:
			draw_pos.x += speed;
			width -= 2 * speed;
			break;
		case gobj::TURN_BACK:
			draw_pos.x -= speed;
			width += 2 * speed;
			break;
		default:
			std::cerr << "Fatal error: invalid flag in method \"turn_over_dx\" from class \"Game_button\"" << std::endl;
			exit(-1);
			break;
		}

	}

	void Game_button::turn_over_parallel()
	{
		sf::Clock clock;
		float delay = 0.001;
		float timer = 0;
		while (width > 2 * frame_w)
		{
			clock.restart();
			timer += clock.getElapsedTime().asSeconds();
			if (timer > delay)
			{
				turn_over_dx(TURN_TO);
				timer = 0;
			}
		}
		std::swap(sprite, second_sprite);
		while (width < back_width)
		{
			clock.restart();
			timer += clock.getElapsedTime().asSeconds();
			if (timer > delay)
			{
				turn_over_dx(TURN_BACK);
				timer = 0;
			}
		}
		draw_pos.x = back_draw_pos.x;
		draw_pos.y = back_draw_pos.y;
	}

	void Game_button::set_background(sf::Sprite& back)
	{
		background = back;
	}

	void Game_button::set_all_fields(const size_t w, const size_t h, const size_t fr_w, sf::Sprite& s, sf::Sprite& sec_s, sf::Sprite& back, const sf::Vector2i& start_vec, const size_t back_w, const sf::Vector2i& back_start_vect, const int new_id)
	{
		draw_pos = start_vec;
		width = w;
		height = h;
		frame_w = fr_w;
		sprite = s;
		second_sprite = sec_s;
		background = back;
		back_width = back_w;
		back_draw_pos = back_start_vect;
		id = new_id;
	}

	void Game_button::print_all_fields() const
	{
		std::cout << "Width: " << width << "\tHeight: " << height << "\tStart_x: " << draw_pos.x << "\tStart_y: " << draw_pos.y << std::endl;
	}

	sf::Sprite& Game_button::get_sprite() const
	{
		return sprite;
	}

	int Game_button::get_id() const
	{
		return id;
	}

	bool Game_button::is_enable_for_turn_over() const
	{
		return is_enablle_for_turn_over_flag;
	}

	void Game_button::make_disable_for_turn_over()
	{
		is_enablle_for_turn_over_flag = false;
		sprite = background;
		second_sprite = background;
	}

	Game_grid::Game_grid(const size_t c_x, const size_t c_y, const size_t one_x, const size_t one_y, const size_t fr_w, std::string** tex_name, std::string** sec_tex_name, std::string** back_name, const sf::Vector2i& start_p)
		:	count_x(c_x), count_y(c_y), start_pos(start_p)
	{
		grid = new Game_button*[count_y];
		tex_array = new sf::Texture*[count_y];
		sec_tex_array = new sf::Texture * [count_y];
		back_tex_array = new sf::Texture * [count_y];
		for (int i = 0; i < count_y; i++)
		{
			grid[i] = new Game_button[count_x];
			tex_array[i] = new sf::Texture[count_x];
			sec_tex_array[i] = new sf::Texture[count_x];
			back_tex_array[i] = new sf::Texture[count_x];
			for (int j = 0; j < count_x; j++)
			{
				tex_array[i][j].loadFromFile(tex_name[i][j]);
				sec_tex_array[i][j].loadFromFile(sec_tex_name[i][j]);
				back_tex_array[i][j].loadFromFile(back_name[i][j]);
				sf::Sprite sprite, sec_sprite, back_sprite;
				sprite.setTexture(tex_array[i][j]);
				sec_sprite.setTexture(sec_tex_array[i][j]);
				back_sprite.setTexture(back_tex_array[i][j]);
				grid[i][j].set_all_fields(one_x, one_y, fr_w, sprite, sec_sprite, back_sprite, sf::Vector2i(start_pos.x + j * one_x, start_pos.y + i * one_y), one_x, sf::Vector2i(start_pos.x + j * one_x, start_pos.y + i * one_y), get_id_from_name(sec_tex_name[i][j]));
			}
		}
	}

	void Game_grid::draw(sf::RenderWindow& window) const
	{
		for (int i = 0; i < count_y; i++)
		{
			for (int j = 0; j < count_x; j++)
			{
				grid[i][j].draw(window);
			}
		}
	}

	void Game_grid::draw_background(sf::RenderWindow& window) const
	{
		for (int i = 0; i < count_y; i++)
		{
			for (int j = 0; j < count_x; j++)
			{
				grid[i][j].draw_background(window);
			}
		}
	}

	Game_grid::~Game_grid()
	{
		for (int i = 0; i < count_y; i++)
		{
			delete[] tex_array[i];
			delete[] sec_tex_array[i];
			delete[] back_tex_array[i];
			delete[] grid[i];
		}
		delete[] tex_array;
		delete[] sec_tex_array;
		delete[] back_tex_array;
		delete[] grid;
	}

	sf::Texture& Game_grid::get_texture(const size_t i, const size_t j) const
	{
		return tex_array[i][j];
	}

	Game_button* Game_grid::operator[](const int index)
	{
		return grid[index];
	}

	int get_id_from_name(std::string name)
	{
		std::string str_id;
		for (char i : name)
		{
			if (i != '.')
			{
				str_id += i;
			}
			else
			{
				break;
			}
		}
		return boost::lexical_cast<int>(str_id);
	}

	Time_bar::Time_bar(const size_t w, const size_t h, const size_t fr_w, sf::Sprite& s, const sf::Vector2i& start_p, sf::Sprite& t_sprite, size_t t_width, const double wh_time, const double del)
		: Table(w, h, fr_w, s, start_p), time_sprite(t_sprite), time_width(t_width), whole_time(wh_time), delay(del)
	{
		speed = width / (whole_time * 1000);
		std::cout << "speed = " << speed << std::endl;
	}

	void Time_bar::draw(sf::RenderWindow& window) const
	{
		Table::draw(window);
		time_sprite.setScale(sf::Vector2f(static_cast<float>(time_width) / standart_x, static_cast<float>(height - 2 * frame_w) / standart_y));
		time_sprite.setPosition(sf::Vector2f(draw_pos.x + frame_w, draw_pos.y + frame_w));
		//std::cout << "Scale:\t" << static_cast<float>(time_width) / standart_x << "\t" << static_cast<float>(height - 2 * frame_w) / standart_y << std::endl;
		window.draw(time_sprite);
	}

	void Time_bar::update()
	{
		if (is_valid_to_update == false)
		{
			return;
		}
		if (clock.getElapsedTime().asSeconds() > delay)
		{
			size_t add_time = static_cast<size_t>(speed * clock.getElapsedTime().asSeconds() * 1000);
			if (add_time)
			{
				time_width += static_cast<size_t>(speed * clock.getElapsedTime().asSeconds() * 1000);
				//std::cout << "time_width:\t" << time_width << std::endl;
				clock.restart();
			}
		}
		if (time_width >= width - 2 * frame_w)
		{
			time_width = width - 2 * frame_w;
			is_valid_to_update = false;
		}
	}

	bool Time_bar::is_enable_to_update() const
	{
		return is_valid_to_update;
	}

}	//	namespace gobj