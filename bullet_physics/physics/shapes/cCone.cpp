/*
* Name:		cCone.h
* Author:	Daniel Maclam
* Date		2019-03-23 12:39 AM
*/

#include "cCone.h"

physics::shapes::cCone::cCone(float radius, float height)
{
	this->shape = new btConeShape(radius,height);
}

btConeShape* physics::shapes::cCone::get_shape()
{
	return this->shape;
}
