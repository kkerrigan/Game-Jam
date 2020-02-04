#pragma once

#define DLL_EXPORT extern "C" _declspec(dllexport)
#include "iRigidBody.h"
#include "../defs/sRigidBodyDef.h"
#include "iHeightMapTerrain.h"

namespace dsm {
	struct sHeightmapDef;
}

namespace physics
{
	namespace interfaces
	{
		class iPhysicsWorld;
		/*!	\class	iPhysicsFactory
			\brief  
			\author Daniel Maclam
			\date	2019-02-01 1:54 PM
			*/
		class iPhysicsFactory
		{
		public:
			virtual ~iPhysicsFactory() = default;

			virtual iPhysicsWorld* create_physics_world() = 0;
			virtual iRigidBody* create_rigid_body(defs::sRigidBodyDef def) = 0;
			virtual iHeightMapTerrain* create_height_map_terrain(dsm::sHeightmapDef def) = 0;
		};

		DLL_EXPORT iPhysicsFactory* create_factory();
		using f_create_physics_factory = iPhysicsFactory*(*)();
	}
}
