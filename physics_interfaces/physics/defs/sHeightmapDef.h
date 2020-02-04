#pragma once
#include <vector>
#include "sHeightmapVertex.h"

namespace dsm
{
	/*!	\class	sHeightmapDef
		\brief  
		\author Daniel Maclam
		\date	2019-04-14 7:02 PM
		*/
	struct sHeightmapDef
	{
		int height; //pixels
		int width;	//pixels
		float min_height;
		float max_height;
		float scale;

		std::vector<sHeightmapVertex> data;
	};
}
