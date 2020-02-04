/*
* Name:		cMazeHelpers.h
* Author:	Daniel Maclam
* Date		2019-03-01 12:49 AM
*/

#include "cMazeHelpers.h"
#include <vector>
#include "cMazeMaker.h"

bool cMazeHelpers::find_closest_cube(std::vector< std::vector< std::vector< cMazeMaker::maze_cube_info > > > maze, glm::vec3 position, unsigned long long* cube_id)
{
	//Gems - Question #3 - helper to find the cube id that you are closest to
	//Uses the AABB hash table so we can have O(1) lookups on the cubes

	position.x = (floor((position.x + 2) / ((float)2 * 2))) * 2 * 2;
	position.y = (floor((position.y + 2) / ((float)2 * 2))) * 2 * 2;
	position.z = (floor((position.z + 2) / ((float)2 * 2))) * 2 * 2;

	unsigned long long theABS_X = (unsigned long long(fabs(position.x)));
	unsigned long long theABS_Y = (unsigned long long(fabs(position.y)));
	unsigned long long theABS_Z = (unsigned long long(fabs(position.z)));

	// Add the "sign" digit:
	// If +ve, then the sign is 0, eg: 193 would be:  000193   (000 193)
	// If -ve, then the sign is 1, eg: -193 would be: 100193   (100 193)
	if (position.x < 0.0f) { theABS_X += 100000; }	// Sets 6th digit to 1
	if (position.y < 0.0f) { theABS_Y += 100000; }	// Sets 6th digit to 1
	if (position.z < 0.0f) { theABS_Z += 100000; }	// Sets 6th digit to 1

	*cube_id =
		theABS_Z
		+ (theABS_Y * 1000000)				// Shift the y to the 7th digit
		+ (theABS_X * 1000000 * 1000000);	// Shift the x to the 13th


	return true;
}
