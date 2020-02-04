#pragma once
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include <vector>
#include "physics/defs/sHeightmapVertex.h"
#include "physics/interfaces/iHeightMapTerrain.h"
#include "BulletCollision/CollisionShapes/btTriangleMeshShape.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

namespace physics
{
	namespace shapes
	{
		/*!	\class	cHeightmapTerrain
			\brief
			\author Daniel Maclam
			\date	2019-04-14 6:38 PM
			*/
		class cHeightmapTerrain : public interfaces::iHeightMapTerrain
		{
			btRigidBody* r_body;
			btBvhTriangleMeshShape* shape;
		public:
			

			std::vector<dsm::sHeightmapVertex> data;

			cHeightmapTerrain(int width, int height,std::vector<dsm::sHeightmapVertex> data, float scale, float min_height, float max_height);

			btRigidBody* get_shape() override; 
		};
	}
}
