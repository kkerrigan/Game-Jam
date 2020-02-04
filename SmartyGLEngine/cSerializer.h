#pragma once
#include <string>
#include "sWindowSettings.h"


/*!	\class	cSerializer
	\brief  the soon to be home of all the serialization functions
	\author Daniel Maclam
	\date	2019-02-17 6:21 PM
	*/
class cSerializer
{
public:

	static bool load(const std::string& file, sWindowSettings* settings);
};

