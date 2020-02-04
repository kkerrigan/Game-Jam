/*
* Name:		cCapsule.h
* Author:	Daniel Maclam
* Date		2019-03-22 11:15 PM
*/

#include "cCapsule.h"

physics::shapes::cCapsule::cCapsule(float radius, float height)
{
	this->shape = new btCapsuleShape(radius, height);
}
