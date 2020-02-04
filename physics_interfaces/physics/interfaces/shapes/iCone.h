#pragma once
#include "../iShape.h"

namespace physics
{
	namespace shapes
	{
		/*!	\class	iCone
		\brief
		\author Daniel Maclam
		\date	2019-03-23 12:36 AM
		*/
		class iCone : public physics::interfaces::iShape
		{
		public:
			shape_types get_shape_type() override
			{
				return shape_types::cone;
			}
		};
	}
}

