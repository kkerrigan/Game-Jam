/*
* Name:		cSphere.h
* Author:	Daniel Maclam
* Date		2019-02-01 5:50 PM
*/

#include "cSphere.h"

physics::interfaces::iShape::shape_types physics::shapes::cSphere::get_shape_type()
{
	return iShape::shape_types::sphere;
}

float physics::shapes::cSphere::get_radius()
{
	return this->radius;
}
