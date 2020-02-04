/*
* Name:		cPhysicsFactory.h
* Author:	Daniel Maclam
* Date		2019-02-01 2:06 PM
*/

#include "cPhysicsFactory.h"
#include "cPhysicsWorld.h"
#include "cRigidBody.h"
#include "shapes/cSphere.h"
#include "shapes/cPlane.h"

physics::interfaces::iPhysicsWorld* physics::cPhysicsFactory::create_physics_world()
{
	return new cPhysicsWorld();
}

physics::interfaces::iRigidBody* physics::cPhysicsFactory::create_rigid_body(defs::sRigidBodyDef def)
{
	interfaces::iShape* shape = nullptr;
	if(def.shape_type == interfaces::iShape::shape_types::sphere)
	{
		shape = new shapes::cSphere(def.radius);
	}
	else if(def.shape_type == interfaces::iShape::shape_types::plane)
	{
		shape = new shapes::cPlane(def.p1, def.p2,def.p3);
	}
	
	return new cRigidBody(
		def.position,
		def.scale,
		def.orientation,
		def.acceleration,
		def.velocity,
		shape,
		def.mass
	);
}

DLL_EXPORT physics::interfaces::iPhysicsFactory* physics::interfaces::create_factory()
{
	return new cPhysicsFactory();
}