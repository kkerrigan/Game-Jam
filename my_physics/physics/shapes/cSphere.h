#pragma once
#include <physics/interfaces/shapes/iSphere.h>
#include <glm/vec3.hpp>

namespace physics
{
	namespace shapes
	{
		/*!	\class	cSphere
			\brief  
			\author Daniel Maclam
			\date	2019-02-01 5:50 PM
			*/
		class cSphere : public interfaces::shapes::iSphere
		{
		private:
			float radius;
		public:
			cSphere(const float radius = 1.0f): radius(radius) {}
			shape_types get_shape_type() override;
			float get_radius() override;
		};
	}
}
