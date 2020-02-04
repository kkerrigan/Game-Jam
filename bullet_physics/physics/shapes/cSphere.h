#pragma once
#include "physics/interfaces/shapes/iSphere.h"
#include <btBulletDynamicsCommon.h>
namespace physics
{
	/*!	\class	cSphere
		\brief
		\author Daniel Maclam
		\date	2019-02-11 7:48 PM
		*/
	class cSphere : public interfaces::shapes::iSphere
	{
		btCollisionShape* sphere;

	public:
		cSphere(const float radius = 1.0f);
		~cSphere();
		shape_types get_shape_type() override;
		virtual float get_radius() override;

		btCollisionShape* get_shape() const
		{
			return this->sphere;
		}
	};
}
