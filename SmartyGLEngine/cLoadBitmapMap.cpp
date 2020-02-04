/*
* Name:		cLoadBitmapMap.h
* Author:	Daniel Maclam
* Date		2019-03-14 2:36 PM
*/

#include "cLoadBitmapMap.h"

void cLoadBitmapMap::initialize()
{
	this->loaded_colors[color_types::traversable] = glm::ivec3(255, 255, 255);
	this->loaded_colors[color_types::non_traversable] = glm::ivec3(0, 0, 0);
	this->loaded_colors[color_types::gatherer_start_position] = glm::ivec3(0, 255, 0);
	this->loaded_colors[color_types::resource] = glm::ivec3(255, 0, 0);
	this->loaded_colors[color_types::base] = glm::ivec3(0, 0, 255);
}

unsigned char* cLoadBitmapMap::load_data(const std::string& file)
{
	FILE* f = fopen(file.c_str(), "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); //header (54 bytes)

	//get the width and height from the header
	width = *reinterpret_cast<int*>(&info[18]);
	height = *reinterpret_cast<int*>(&info[22]);

	this->num_of_pixels = width * height;

	//calculate the size (number of pixels * 3 color weights(rgb)
	const int size = 3 * width * height;

	if(data != nullptr)
	{
		delete data;
	}

	data = new unsigned char[size]; //3 bytes per pixel
	fread(data, sizeof(unsigned char), size, f); //read all the data at once
	fclose(f);

	for (int i = 0; i < size; i += 3)
	{
		const unsigned char tmp = data[i];
		data[i] = data[i + 2];
		data[i + 2] = tmp;
	}

	cLoadBitmapMap::flip_bmp(data, width,height);

	return data;
}

bool cLoadBitmapMap::flip_bmp(unsigned char* in_buffer, const unsigned width, const int height)
{
	if (in_buffer == nullptr)
		return false;

	const unsigned buffer_size = width * 3;
	unsigned char* tb1 = new unsigned char[buffer_size];
	if (tb1 == nullptr)
	{
		return false;
	}

	unsigned char* tb2 = new unsigned char[buffer_size];
	if (tb2 == nullptr)
	{
		delete[] tb1;
		return false;
	}

	long off1 = 0;
	long off2 = 0;

	for (int row_count = 0; row_count < (height) / 2; row_count++)
	{
		off1 = row_count * buffer_size;
		off2 = ((height - 1) - row_count) * buffer_size;

		memcpy(tb1, in_buffer + off1, buffer_size * sizeof(unsigned char));
		memcpy(tb2, in_buffer + off2, buffer_size * sizeof(unsigned char));
		memcpy(in_buffer + off1, tb2, buffer_size * sizeof(unsigned char));
		memcpy(in_buffer + off2, tb1, buffer_size * sizeof(unsigned char));
	}

	delete[] tb1;
	delete[] tb2;

	return true;
}

bool cLoadBitmapMap::is_color_type(float r, float g, float b, color_types type)
{
	//find it in the loaded colors
	const std::map<color_types, glm::ivec3>::iterator color = this->loaded_colors.find(type);
	if (color == this->loaded_colors.end())
	{
		return false;
	}

	return r == color->second.r && g == color->second.g && b == color->second.b;
}
