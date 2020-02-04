#pragma once
namespace physics
{
	namespace interfaces
	{
		class iRigidBody;
		class iHeightMapTerrain;

		/*!	\class	iPhysicsWorld
			\brief  
			\author Daniel Maclam
			\date	2019-02-01 1:50 PM
			*/
		class iPhysicsWorld
		{
		public:
			virtual ~iPhysicsWorld() = default;

			virtual void register_rigid_body(iRigidBody* body) = 0;
			virtual void unregister_rigid_body(iRigidBody* body) = 0;
			virtual void register_heightmap_terrain(iHeightMapTerrain* terrain) = 0;
			virtual void update(float delta_time) = 0;

		};
	}
}
