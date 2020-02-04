/*
* Name:		cSphere.h
* Author:	Daniel Maclam
* Date		2019-02-11 7:48 PM
*/

#include "cSphere.h"
#include "../bullet_adapters/bullet_adapters.h"

physics::cSphere::cSphere(const float radius)
{
	this->sphere = new btSphereShape(bullet_adapters::to_bullet(radius));
}

physics::cSphere::~cSphere()
{
	delete this->sphere;
}

physics::interfaces::iShape::shape_types physics::cSphere::get_shape_type()
{
	return iShape::sphere;
}

float physics::cSphere::get_radius()
{
	return bullet_adapters::to_glm(static_cast<btSphereShape*>(this->sphere)->getRadius());
}
