#pragma once
#include "../iShape.h"
#include <glm/vec3.hpp>

namespace physics
{
	namespace interfaces
	{
		namespace shapes
		{
			/*!	\class	iPlane
			\brief
			\author Daniel Maclam
			\date	2019-02-01 6:34 PM
			*/
			class iPlane : public iShape
			{
			public:
				virtual shape_types get_shape_type() override = 0;
				virtual glm::vec3 get_normal() = 0;
				virtual float get_constant() = 0;
			};
		}
	}
}
