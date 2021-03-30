#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

const sf::Vector2i std_vect(0, 0);
static sf::Sprite std_sprite;

namespace gobj
{

	enum Flags_for_turn
	{
		TURN_TO,
		TURN_BACK
	};

	class Table
	{
	protected:
		sf::Sprite& sprite;
		sf::Vector2i draw_pos;
		size_t width;
		size_t height;
		size_t frame_w;
	public:
		Table();
		Table(const size_t, const size_t, const size_t, sf::Sprite&, const sf::Vector2i&);	//	whole width, whole height, frame_w
		Table(sf::RenderWindow&, const size_t, const size_t, sf::Sprite&, const sf::Vector2i& = std_vect);					//	width_screen, heigth_screen
		virtual void draw(sf::RenderWindow&) const;
	};

	class Game_button : public Table
	{
		sf::Sprite& background;
		sf::Sprite& second_sprite;
		sf::Vector2i back_draw_pos;
		size_t back_width;
		int id;
		bool is_enablle_for_turn_over_flag = true;
	public:
		Game_button();
		Game_button(const size_t, const size_t, const size_t, sf::Sprite&, sf::Sprite&, sf::Sprite&, const sf::Vector2i&, const int = 0);
		Game_button(sf::RenderWindow&, const size_t, const size_t, sf::Sprite&, sf::Sprite&, sf::Sprite&, const sf::Vector2i& = std_vect, const int = 0);
		Game_button(Game_button&&);
		void draw_background(sf::RenderWindow&) const;
		void set_background(sf::Sprite&);
		bool is_mouse_this(sf::RenderWindow&) const;
		void draw(sf::RenderWindow&) const;
		void turn_over(sf::RenderWindow&);
		void turn_over_parallel();
		void turn_over_dx(Flags_for_turn);
		void set_all_fields(const size_t, const size_t, const size_t, sf::Sprite&, sf::Sprite&, sf::Sprite&, const sf::Vector2i&, const size_t, const sf::Vector2i&, const int = 0);
		void print_all_fields() const;
		sf::Sprite& get_sprite() const;
		int get_id() const;
		bool is_enable_for_turn_over() const;
		void make_disable_for_turn_over();
	};

	class Game_grid
	{
		Game_button** grid;
		sf::Texture** tex_array;
		sf::Texture** sec_tex_array;
		sf::Texture** back_tex_array;
		size_t count_x;
		size_t count_y;
		sf::Vector2i start_pos;
	public:
		//Game_grid(const size_t, const size_t, const size_t, std::string**, std::string**, std::string**);	//	window, width_screen, height_screen, frame_w, texture_names, second_texture_names, background_names
		Game_grid(const size_t, const size_t, const size_t, const size_t, const size_t, std::string**, std::string**, std::string**, const sf::Vector2i&);	//	count_x, count_y, one_size_x, one_size_y, frame_w, texture_names, second_texture_names, background_names, start_pos
		~Game_grid();
		sf::Texture& get_texture(const size_t, const size_t) const;
		void draw(sf::RenderWindow&) const;
		void draw_background(sf::RenderWindow&) const;
		Game_button* operator[](const int);
	};

	class Time_bar : Table
	{
		sf::Sprite& time_sprite;
		size_t time_width;		//	width of bar in this moment
		double whole_time;
		double speed;		//in pix/ms
		sf::Clock clock;
		double delay;
		bool is_valid_to_update = true;
	public:
		Time_bar(const size_t w, const size_t h, const size_t fr_w, sf::Sprite& s, const sf::Vector2i& start_p, sf::Sprite&, size_t, const double, const double = 0.001);
		void draw(sf::RenderWindow&) const;
		void update();
		bool is_enable_to_update() const;
	};

}	//	namespace tbl