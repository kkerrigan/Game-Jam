#pragma once
#include "iBehaviour.h"

/*!	\class	cWaitBehaviour
	\brief
	\author Daniel Maclam
	\date	2019-03-19 10:29 PM
	*/
class cWaitBehaviour : public iBehaviour
{
	float time_elapsed;
	float wait_time;
public:
	cWaitBehaviour(float wait);
	void update(cEntity* entity, float delta_time) override;
	bool is_finished() const;
};

