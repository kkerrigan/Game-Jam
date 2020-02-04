/*
* Name:		cPlane.h
* Author:	Daniel Maclam
* Date		2019-02-01 6:37 PM
*/
#include <glm/glm.hpp>
#include "cPlane.h"

physics::shapes::cPlane::cPlane(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	this->normal = glm::normalize(glm::cross(b-a, c-a));
	this->constant = glm::dot(this->normal, a);
}

physics::interfaces::iShape::shape_types physics::shapes::cPlane::get_shape_type()
{
	return  iShape::shape_types::plane;
}

glm::vec3 physics::shapes::cPlane::get_normal()
{
	return this->normal;
}

float physics::shapes::cPlane::get_constant()
{
	return this->constant;
}
