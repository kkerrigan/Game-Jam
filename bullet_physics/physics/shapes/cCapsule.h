#pragma once
#include "physics/interfaces/shapes/iCapsule.h"
#include "BulletCollision/CollisionShapes/btCapsuleShape.h"

namespace physics
{
	namespace shapes
	{
		/*!	\class	cCapsule
		\brief
		\author Daniel Maclam
		\date	2019-03-22 11:15 PM
		*/
		class cCapsule : public physics::shapes::iCapsule
		{
			btCapsuleShape* shape;

		public:
			cCapsule(float radius, float height);
			btCapsuleShape* get_shape()
			{
				return this->shape;
			}
		};
	}
}



