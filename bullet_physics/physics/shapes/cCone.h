#pragma once
#include "physics/interfaces/shapes/iCone.h"
#include "BulletCollision/CollisionShapes/btConeShape.h"

namespace physics
{
	namespace shapes
	{
		/*!	\class	cCone
		\brief
		\author Daniel Maclam
		\date	2019-03-23 12:39 AM
		*/
		class cCone : public physics::shapes::iCone
		{
			btConeShape* shape;
		public:
			cCone(float radius, float height);
			btConeShape* get_shape();
		};

	}
}

