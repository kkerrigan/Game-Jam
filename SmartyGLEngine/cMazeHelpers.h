#pragma once
#include <glm/vec3.hpp>
#include <vector>
#include "cMazeMaker.h"


/*!	\class	cMazeHelpers
	\brief
	\author Daniel Maclam
	\date	2019-03-01 12:49 AM
	*/
class cMazeHelpers
{
public:
	static bool find_closest_cube(std::vector< std::vector< std::vector< cMazeMaker::maze_cube_info > > > maze, glm::vec3 position, unsigned long long* cube_id);

};
