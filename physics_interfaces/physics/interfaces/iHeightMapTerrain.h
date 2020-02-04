#pragma once
class btRigidBody;
namespace physics
{
	namespace interfaces
	{
		/*!	\class	iHeightMapTerrain
		\brief
		\author Daniel Maclam
		\date	2019-04-14 6:55 PM
		*/
		class iHeightMapTerrain
		{
		public:
			virtual ~iHeightMapTerrain() = default;
			virtual btRigidBody* get_shape() = 0;
		};
	}
}
