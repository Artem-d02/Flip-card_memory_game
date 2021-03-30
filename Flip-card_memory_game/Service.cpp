#include <iostream>
#include "Service.h"
#include <cstring>

namespace svc
{

	Triple_string_arr::Triple_string_arr(const size_t count_x, const size_t count_y)
		:	x_count_string(count_x), y_count_string(count_y)
	{
		texture_names = new std::string*[y_count_string];
		second_texture_names = new std::string*[y_count_string];
		background_texture_names = new std::string* [y_count_string];
		for (int i = 0; i < y_count_string; i++)
		{
			texture_names[i] = new std::string[x_count_string];
			second_texture_names[i] = new std::string[x_count_string];
			background_texture_names[i] = new std::string[x_count_string];
		}
	}

	Triple_string_arr::~Triple_string_arr()
	{
		for (int i = 0; i < y_count_string; i++)
		{
			delete[] texture_names[i];
			delete[] second_texture_names[i];
			delete[] background_texture_names[i];
		}
		delete[] texture_names;
		delete[] second_texture_names;
		delete[] background_texture_names;
	}

	size_t Triple_string_arr::get_x_count() const
	{
		return x_count_string;
	}

	size_t Triple_string_arr::get_y_count() const
	{
		return y_count_string;
	}

	std::string** Triple_string_arr::operator[](Triple_string_arr_index index)
	{
		switch (index)
		{
		case(TEX_NAMES):
			return texture_names;
		case(SEC_TEX_NAMES):
			return second_texture_names;
		case(BACK_TEX_NAMES):
			return background_texture_names;
		default:
			return nullptr;
		}
	}

	Mutex_arr::Mutex_arr(const size_t x_count_mutex, const size_t y_count_mutex)
		: x_count(x_count_mutex), y_count(y_count_mutex)
	{
		mutexes = new std::mutex * [y_count];
		for (int i = 0; i < y_count; i++)
		{
			mutexes[i] = new std::mutex[x_count];
		}
	}

	Mutex_arr::~Mutex_arr()
	{
		for (int i = 0; i < y_count; i++)
		{
			delete[] mutexes[i];
		}
		delete[] mutexes;
	}

	std::mutex* Mutex_arr::operator[](const size_t index)
	{
		return mutexes[index];
	}

}	//	namespace svc