/*
* Name:		cPlane.h
* Author:	Daniel Maclam
* Date		2019-02-11 7:58 PM
*/

#include "cPlane.h"
#include <glm/glm.hpp>
#include "../bullet_adapters/bullet_adapters.h"

physics::shapes::cPlane::cPlane(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	const glm::vec3 normal = glm::vec3(0.0f,1.0f,0.0f);//glm::normalize(glm::cross(b - a, c - a));
	const float constant = glm::dot(normal, a);

	this->plane = new btStaticPlaneShape(bullet_adapters::to_bullet(normal), bullet_adapters::to_bullet(0));
}

physics::interfaces::iShape::shape_types physics::shapes::cPlane::get_shape_type()
{
	return  iShape::shape_types::plane;
}

glm::vec3 physics::shapes::cPlane::get_normal()
{
	return bullet_adapters::to_glm(this->plane->getPlaneNormal());
}

float physics::shapes::cPlane::get_constant()
{
	return bullet_adapters::to_glm(this->plane->getPlaneConstant());
}