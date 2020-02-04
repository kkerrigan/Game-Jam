/*
* Name:		cPhysicsComponent.h
* Author:	Daniel Maclam
* Date		2019-02-01 3:17 PM
*/

#include "cPhysicsComponent.h"
#include "cEngine.h"

physics::interfaces::iRigidBody* cPhysicsComponent::get_rigid_body()
{
	return this->rigid_body;
}

void cPhysicsComponent::configure_component(physics::interfaces::iRigidBody* r_body)
{
	this->rigid_body = r_body;
}
