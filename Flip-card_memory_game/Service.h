#pragma once
#include <iostream>
#include <cstring>
#include <mutex>
#include "Game_objects.h"
#include "SFML/Graphics.hpp"

namespace svc
{
	enum Triple_string_arr_index
	{
		TEX_NAMES,
		SEC_TEX_NAMES,
		BACK_TEX_NAMES
	};

	class Triple_string_arr
	{
		std::string** texture_names;
		std::string** second_texture_names;
		std::string** background_texture_names;
		size_t x_count_string;
		size_t y_count_string;
	public:
		Triple_string_arr(const size_t, const size_t);
		~Triple_string_arr();
		size_t get_x_count() const;
		size_t get_y_count() const;
		std::string** operator[](Triple_string_arr_index);
	};

	class Mutex_arr
	{
		std::mutex** mutexes;
		size_t x_count;
		size_t y_count;
	public:
		Mutex_arr(const size_t, const size_t);
		~Mutex_arr();
		std::mutex* operator[](const size_t);
	};

}	//	namespace svc