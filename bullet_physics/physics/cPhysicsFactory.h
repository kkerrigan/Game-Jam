#pragma once
#include <physics/interfaces/iPhysicsFactory.h>
#include "physics/defs/sHeightmapDef.h"

namespace physics
{
	/*!	\class	cPhysicsFactory
		\brief
		\author Daniel Maclam
		\date	2019-02-01 2:06 PM
		*/
	class cPhysicsFactory : public interfaces::iPhysicsFactory
	{
	public:
		interfaces::iPhysicsWorld* create_physics_world() override;
		interfaces::iRigidBody* create_rigid_body(defs::sRigidBodyDef def) override;
		interfaces::iHeightMapTerrain* create_height_map_terrain(dsm::sHeightmapDef def) override;
		
	};
}
