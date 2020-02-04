#pragma once

#include <string>
struct sTextureInfo
{
	std::string name;
	int cachedTextureID; 	// Set to -1 by default
	float strength;		// Set to 0.0f by default
};
