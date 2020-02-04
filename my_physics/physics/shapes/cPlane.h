#pragma once
#include "physics/interfaces/shapes/iPlane.h"

namespace physics
{
	namespace shapes
	{
		/*!	\class	cPlane
			\brief  
			\author Daniel Maclam
			\date	2019-02-01 6:37 PM
			*/
		class cPlane : public interfaces::shapes::iPlane
		{
			glm::vec3 normal;
			float constant;

		public:			 
			cPlane(glm::vec3 a, glm::vec3 b, glm::vec3 c);

			shape_types get_shape_type() override;
			glm::vec3 get_normal() override;
			float get_constant() override;
		};
	}
}
