/*
* Name:		cPhysicsFactory.h
* Author:	Daniel Maclam
* Date		2019-02-11 9:47 PM
*/

#include "cPhysicsFactory.h"
#include "cPhysicsWorld.h"
#include "cRigidBody.h"
#include "shapes/cSphere.h"
#include "shapes/cPlane.h"
#include "bullet_adapters/bullet_adapters.h"
#include "shapes/cHeightmapTerrain.h"
#include "shapes/cCone.h"
#include "shapes/cCapsule.h"
#include "shapes/cBox.h"

physics::interfaces::iPhysicsWorld* physics::cPhysicsFactory::create_physics_world()
{
	return new cPhysicsWorld();
}

physics::interfaces::iHeightMapTerrain* physics::cPhysicsFactory::create_height_map_terrain(dsm::sHeightmapDef def)
{
	return new physics::shapes::cHeightmapTerrain(
		def.height,
		def.width,
		def.data,
		def.scale,
		def.min_height,
		def.max_height
	);
}

physics::interfaces::iRigidBody* physics::cPhysicsFactory::create_rigid_body(defs::sRigidBodyDef def)
{
	btRigidBody* bullet_body = nullptr;

	if (def.shape_type == interfaces::iShape::shape_types::sphere)
	{
		cSphere* sphere = new cSphere(def.radius);

		btDefaultMotionState* motions_state = new btDefaultMotionState(btTransform(
			bullet_adapters::to_bullet(bullet_adapters::create_quaternion_from_euler_angles(def.orientation)),
			bullet_adapters::to_bullet(def.position)));
		
		btScalar mass = def.mass;
		btVector3 inertia(0, 0, 0);
		sphere->get_shape()->calculateLocalInertia(mass, inertia);
		btRigidBody::btRigidBodyConstructionInfo rbci(mass, motions_state, sphere->get_shape(), inertia);
		rbci.m_restitution = 1.0f;	//spheres are really bouncy
		bullet_body = new btRigidBody(rbci);
	}
	else if (def.shape_type == interfaces::iShape::shape_types::plane)
	{
		shapes::cPlane* plane = new shapes::cPlane(def.p1, def.p2, def.p3);

		btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(
			bullet_adapters::to_bullet(bullet_adapters::create_quaternion_from_euler_angles(def.orientation)),
			bullet_adapters::to_bullet(def.position))
		);

		btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(
			0,
			motion_state,
			plane->get_shape(),
			btVector3(0, 0, 0) //no inertia
		);
		groundRigidBodyCI.m_restitution = 0.5f;

		bullet_body = new btRigidBody(groundRigidBodyCI);
	}
	else if (def.shape_type == interfaces::iShape::shape_types::box)
	{
		shapes::cBox* box = new shapes::cBox(def.half_lengths);

		btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(
			bullet_adapters::to_bullet(bullet_adapters::create_quaternion_from_euler_angles(def.orientation)), 
			bullet_adapters::to_bullet(def.position))
		);

		btVector3 inertia(0, 0, 0);
		box->get_shape()->calculateLocalInertia(def.mass, inertia);

		btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(
			def.mass,
			motion_state,
			box->get_shape(),
			inertia 
		);
		groundRigidBodyCI.m_restitution = 0.5f;

		bullet_body = new btRigidBody(groundRigidBodyCI);
	}
	else if (def.shape_type == interfaces::iShape::shape_types::capsule)
	{
		shapes::cCapsule* capsule = new shapes::cCapsule(def.radius, def.height);

		btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(
			bullet_adapters::to_bullet(bullet_adapters::create_quaternion_from_euler_angles(def.orientation)), 
			bullet_adapters::to_bullet(def.position))
		);

		btVector3 inertia(0, 0, 0);

		btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(
			def.mass,
			motion_state,
			capsule->get_shape(),
			inertia //no inertia
		);
		groundRigidBodyCI.m_restitution = 0.5f;

		bullet_body = new btRigidBody(groundRigidBodyCI);

		//bullet_body->setDamping(0.90, 0.0);
	}
	else if (def.shape_type == interfaces::iShape::shape_types::cone)
	{
		shapes::cCone* cone = new shapes::cCone(def.radius, def.height);

		btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(
			bullet_adapters::to_bullet(bullet_adapters::create_quaternion_from_euler_angles(def.orientation)), 
			bullet_adapters::to_bullet(def.position))
		);

		btVector3 inertia(0, 0, 0);
		cone->get_shape()->calculateLocalInertia(def.mass, inertia);

		btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(
			def.mass,
			motion_state,
			cone->get_shape(),
			inertia //no inertia
		);
		groundRigidBodyCI.m_restitution = 0.5f;

		bullet_body = new btRigidBody(groundRigidBodyCI);
	}

	cRigidBody* rigid_body = new cRigidBody(bullet_body, def.scale);
	return rigid_body;
}

DLL_EXPORT physics::interfaces::iPhysicsFactory* physics::interfaces::create_factory()
{
	return new cPhysicsFactory();
}
