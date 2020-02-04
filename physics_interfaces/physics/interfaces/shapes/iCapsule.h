#pragma once
#include "../iShape.h"

namespace physics
{
	namespace shapes
	{
		/*!	\class	iCapsule
		\brief
		\author Daniel Maclam
		\date	2019-03-22 11:13 PM
		*/
		class iCapsule : public physics::interfaces::iShape
		{
		public:
			shape_types get_shape_type() override
			{
				return shape_types::capsule;
			}
		};

	}
}

