#pragma once
#include "../iShape.h"

/*!	\class	iBox
	\brief	
	\author Daniel Maclam
	\date	2019-03-22 6:14 PM
	*/
class iBox : public physics::interfaces::iShape
{
public:
	shape_types get_shape_type() override
	{
		return shape_types::box;
	}
};

