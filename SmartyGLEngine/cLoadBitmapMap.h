#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <map>
#include <glm/vec3.hpp>

/*!	\class	cLoadBitmapMap
	\brief
	\author Daniel Maclam
	\date	2019-03-14 2:36 PM
	*/

class cLoadBitmapMap
{
public:
	enum color_types
	{
		traversable,
		non_traversable,
		gatherer_start_position,
		base,
		resource,
		unkown
	};

	unsigned char* data;
	std::map<color_types, glm::ivec3> loaded_colors;
	int width;
	int height;
	int num_of_pixels;

	void initialize();
	unsigned char* load_data(const std::string& file);
	static bool flip_bmp(unsigned char* in_buffer, unsigned width, int height);
	bool is_color_type(float r, float g, float b, color_types type);
};
