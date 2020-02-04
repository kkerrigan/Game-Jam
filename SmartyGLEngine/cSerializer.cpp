/*
* Name:		cSerializer.h
* Author:	Daniel Maclam
* Date		2019-02-17 6:21 PM
*/

#include "cSerializer.h"
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>

bool cSerializer::load(const std::string& file, sWindowSettings* settings)
{
	std::ifstream ifs(file);

	if (!ifs.is_open()) {
		return false;
	}

	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document document;
	document.ParseStream(isw);

	if(document.HasMember("title") && document["title"].IsString())
	{
		settings->title = document["title"].GetString();
	}

	if(document.HasMember("width") && document["width"].IsInt())
	{
		settings->width = document["width"].GetInt();
	}

	if(document.HasMember("height") && document["height"].IsInt())
	{
		settings->height = document["height"].GetInt();
	}

	if(document.HasMember("physics_implementation") && document["physics_implementation"].IsString())
	{
		settings->physics_implementation = document["physics_implementation"].GetString();
	}
	
	return true;
}
