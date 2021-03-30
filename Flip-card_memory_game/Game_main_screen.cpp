#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstring>
#include <thread>
#include <mutex>
#include <vector>
#include <ctime>
#include <boost/lexical_cast.hpp>
#include "Game_objects.h"
#include "Service.h"

namespace gms
{
	const int STD_VALUE_FOR_ID_VECTOR = -1;

	void execute_game(sf::RenderWindow&);
	void initialization_texture_names(svc::Triple_string_arr&);
	void smart_turn_over(gobj::Game_button&, std::mutex&, std::vector<int>&, std::mutex&, size_t*);
	void make_random_pair_array_of_string(std::string**, const size_t, const size_t);

	void make_random_pair_array_of_string(std::string** tex_names, const size_t count_x, const size_t count_y)
	{
		std::srand(time(0));
		for (int i = 0; i < count_y; i++)
		{
			for (int j = 0; j < count_x; j++)
			{
				tex_names[i][j] = boost::lexical_cast<std::string>((i * count_x + j) / 2) + ".png";
				tex_names[i][j] = boost::lexical_cast<std::string>((i * count_x + j) / 2) + ".png";
			}
		}
		for (int i = 0; i < count_x * count_y; i++)
		{
			std::swap(tex_names[std::rand() % count_y][std::rand() % count_x], tex_names[std::rand() % count_y][std::rand() % count_x]);
		}
	}

	void smart_turn_over(gobj::Game_button& but, std::mutex& mut, std::vector<int>& id_vec, std::mutex& mut_for_vec, size_t* count_of_guessed_but_ptr)
	{
		mut.lock();
		if (!but.is_enable_for_turn_over())
		{
			mut.unlock();
			return;
		}
		but.turn_over_parallel();
		mut.unlock();

		static int call_count = 0;
		call_count++;

		static std::vector<gobj::Game_button*> game_but_ptr(2, nullptr);
		static std::mutex mut_for_game_but_ptr;

		static bool flag_for_the_same = false;
		static std::mutex mut_for_same_flag;

		mut_for_game_but_ptr.lock();
		game_but_ptr[0] = game_but_ptr[1];
		game_but_ptr[1] = &but;
		if (game_but_ptr[0] == game_but_ptr[1] && call_count % 2 == 0)
		{
			mut_for_same_flag.lock();
			flag_for_the_same = true;
			mut_for_same_flag.unlock();
		}
		mut_for_game_but_ptr.unlock();

		mut_for_vec.lock();

		id_vec[0] = id_vec[1];
		id_vec[1] = but.get_id();
		if (id_vec[0] == STD_VALUE_FOR_ID_VECTOR)
		{
			mut_for_vec.unlock();
			while (id_vec[0] == STD_VALUE_FOR_ID_VECTOR || id_vec[1] == STD_VALUE_FOR_ID_VECTOR);
			mut_for_vec.lock();
			
			if (id_vec[0] == id_vec[1] && id_vec[0] != STD_VALUE_FOR_ID_VECTOR)
			{
				mut_for_same_flag.lock();
				if (flag_for_the_same)
				{
					id_vec[0] = STD_VALUE_FOR_ID_VECTOR;
					id_vec[1] = STD_VALUE_FOR_ID_VECTOR;
					//id_vec[1] = STD_VALUE_FOR_ID_VECTOR;
					flag_for_the_same = false;
					mut_for_same_flag.unlock();
					mut_for_vec.unlock();
					/*
					mut.lock();
					but.turn_over_parallel();
					mut.unlock();
					*/
					return;
				}
				else
				{
					but.make_disable_for_turn_over();
					(*count_of_guessed_but_ptr)++;
					id_vec[0] = STD_VALUE_FOR_ID_VECTOR;
					id_vec[1] = STD_VALUE_FOR_ID_VECTOR;
					mut_for_same_flag.unlock();
					mut_for_vec.unlock();
					return;
				}
			}
			else
			{
				id_vec[0] = STD_VALUE_FOR_ID_VECTOR;
				mut_for_vec.unlock();
				mut.lock();
				but.turn_over_parallel();
				mut.unlock();
				return;
			}
		}
		if (id_vec[0] == id_vec[1])
		{
			mut_for_same_flag.lock();
			if (flag_for_the_same)
			{
				//id_vec[0] = STD_VALUE_FOR_ID_VECTOR;
				//id_vec[1] = STD_VALUE_FOR_ID_VECTOR;
				//flag_for_the_same = false;
				mut_for_same_flag.unlock();
				mut_for_vec.unlock();
				/*
				mut.lock();
				but.turn_over_parallel();
				mut.unlock();
				*/
				return;
			}
			else
			{
				but.make_disable_for_turn_over();
				(*count_of_guessed_but_ptr)++;
				mut_for_same_flag.unlock();
				mut_for_vec.unlock();
				return;
			}
		}
		if (id_vec[0] != id_vec[1])
		{
			id_vec[1] = STD_VALUE_FOR_ID_VECTOR;
			mut_for_vec.unlock();
			mut.lock();
			but.turn_over_parallel();
			mut.unlock();
			return;
		}
	}

	void initialization_texture_names(svc::Triple_string_arr& name_arr)
	{
		for (int i = 0; i < name_arr.get_y_count(); i++)
		{
			for (int j = 0; j < name_arr.get_x_count(); j++)
			{
				name_arr[svc::TEX_NAMES][i][j] = "Red_rect.png";
				name_arr[svc::BACK_TEX_NAMES][i][j] = "Black_rect.png";
			}
		}
		make_random_pair_array_of_string(name_arr[svc::SEC_TEX_NAMES], name_arr.get_x_count(), name_arr.get_y_count());
	}

	void execute_game(sf::RenderWindow& window)
	{
		sf::Event event;
		window.clear(sf::Color::Black);
		
		const int width_count = 4;
		const int height_count = 4;
		const size_t width_one = 200;
		const size_t height_one = 100;
		const size_t frame_widht = 5;
		const size_t time_bar_w = width_count * width_one;
		const size_t time_bar_h = 20;

		svc::Triple_string_arr tex_names_arr(width_count, height_count);
		initialization_texture_names(tex_names_arr);
		
		gobj::Game_grid my_grid(width_count, height_count, width_one, height_one, frame_widht, tex_names_arr[svc::TEX_NAMES], tex_names_arr[svc::SEC_TEX_NAMES], tex_names_arr[svc::BACK_TEX_NAMES], sf::Vector2i(0, 20));
		
		svc::Mutex_arr my_mutexes(width_count, height_count);

		std::vector<int> check_same_id_vector(2, STD_VALUE_FOR_ID_VECTOR);
		std::mutex check_id_mutex;

		sf::Texture time_bar_back_tex;
		time_bar_back_tex.loadFromFile("Black_rect.png");
		sf::Texture time_bar_tex;
		time_bar_tex.loadFromFile("Time_bar.png");

		sf::Sprite time_bar_back_sprite(time_bar_back_tex);
		sf::Sprite time_bar_sprite(time_bar_tex);

		gobj::Time_bar my_time_bar(time_bar_w, time_bar_h, 0, time_bar_back_sprite, sf::Vector2i(0, 0), time_bar_sprite, 0, 60);		//	last is the time in sec

		size_t count_of_guessed_but = 0;

		while (window.isOpen())
		{

			my_grid.draw_background(window);
			my_grid.draw(window);
			my_time_bar.update();
			my_time_bar.draw(window);

			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					return;
				if (event.type == sf::Event::MouseButtonPressed)
				{
					if (event.key.code == sf::Mouse::Left)
					{
						bool is_pressed_yet = false;
						for (int i = 0; i < height_count; i++)
						{
							for (int j = 0; j < width_count; j++)
							{
								if (my_grid[i][j].is_mouse_this(window))
								{
									//void (gobj::Game_button:: *turn_over_ptr)(sf::RenderWindow&) = &(gobj::Game_button::turn_over);
									std::thread thr(smart_turn_over, std::ref(my_grid[i][j]), std::ref(my_mutexes[i][j]), std::ref(check_same_id_vector), std::ref(check_id_mutex), &count_of_guessed_but);
									thr.detach();
									//my_grid[i][j].turn_over(window);
									is_pressed_yet = true;
									break;
								}
							}
							if (is_pressed_yet)
							{
								break;
							}
						}
					}
				}
				
			}
			window.display();
			if (my_time_bar.is_enable_to_update() == false)
			{
				return;
			}
			if (count_of_guessed_but == height_count * width_count)
			{
				std::cout << "Success!" << std::endl;
				return;
			}
		}
	}
}	//	namespace gms