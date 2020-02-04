#pragma once

#include "btBulletDynamicsCommon.h"
#include "physics/interfaces/iPhysicsWorld.h"
#include "physics/defs/sRigidBodyDef.h"

namespace physics
{
	/*!	\class	cPhysicsWorld
		\brief
		\author Daniel Maclam
		\date	2019-02-11 7:07 PM
		*/
	class cPhysicsWorld : public interfaces::iPhysicsWorld
	{
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* overlappingPairCache;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;
		btAlignedObjectArray<btCollisionShape*> collisionShapes;

	public:
		cPhysicsWorld();
		~cPhysicsWorld();
		void register_rigid_body(physics::interfaces::iRigidBody* body) override;
		void unregister_rigid_body(physics::interfaces::iRigidBody* body) override;
		void register_heightmap_terrain(interfaces::iHeightMapTerrain* terrain) override;

		void update(float delta_time) override;

	};
}
