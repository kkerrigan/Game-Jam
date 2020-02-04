/*
* Name:		cAABB.h
* Author:	Daniel Maclam
* Date		2019-03-22 6:24 PM
*/

#include "cBox.h"
#include "../bullet_adapters/bullet_adapters.h"

physics::shapes::cBox::cBox(glm::vec3 half_extents)
{
	this->shape = new btBoxShape(physics::bullet_adapters::to_bullet(half_extents));
}
