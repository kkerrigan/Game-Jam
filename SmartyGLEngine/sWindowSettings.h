#pragma once

#include <string>

struct sWindowSettings {
	int width;
	int height;
	std::string title;
	std::string physics_implementation;
	sWindowSettings(): width(0), height(0), title(""){}
	sWindowSettings(int width, int height, std::string title) : width(width), height(height), title(title) {}
};