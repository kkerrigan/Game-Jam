#pragma once
#include "cEntity.h"

/*!	\class	iBehaviour
	\brief
	\author Daniel Maclam
	\date	2019-03-03 5:54 PM
	*/
class iBehaviour
{
public:
	virtual ~iBehaviour() = default;
	virtual void update(cEntity* entity, float delta_time) = 0;
};