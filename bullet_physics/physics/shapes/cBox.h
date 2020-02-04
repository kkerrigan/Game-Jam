#pragma once
#include "physics/interfaces/shapes/iBox.h"
#include <glm/vec3.hpp>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "BulletCollision/CollisionShapes/btStaticPlaneShape.h"

namespace physics
{
	namespace shapes
	{
	/*!	\class	cBox
	\brief
	\author Daniel Maclam
	\date	2019-03-22 6:24 PM
	*/
		class cBox : public iBox
		{
		public:
			cBox(glm::vec3 half_extents);
			btBoxShape* shape;

			btBoxShape* get_shape()
			{
				return this->shape;
			}
		};
	}
}


