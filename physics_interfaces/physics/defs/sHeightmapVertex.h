#pragma once
#include <glm/vec3.hpp>

namespace dsm
{
	/*!	\class	sHeightmapVertex
		\brief
		\author Daniel Maclam
		\date	2019-04-14 7:01 PM
		*/
	
	struct sHeightmapVertex
	{
		glm::vec3 vertexes[3];
		int indices[3];
	};
}
