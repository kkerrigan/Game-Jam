#pragma once
#include "../iShape.h"

namespace physics
{
	namespace interfaces
	{
		namespace shapes
		{
			/*!	\class	iSphere
			\brief
			\author Daniel Maclam
			\date	2019-02-01 5:47 PM
			*/
			class iSphere : public iShape
			{
			public:
				virtual ~iSphere(){};
				shape_types get_shape_type() override = 0;
				virtual float get_radius() = 0;
			};
		}
	}
}
