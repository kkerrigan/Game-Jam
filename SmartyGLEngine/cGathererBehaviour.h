#pragma once
#include "iBehaviour.h"


class cScene;

/*!	\class	cGathererBehaviour
	\brief
	\author Daniel Maclam
	\date	2019-03-19 9:46 PM
	*/
class cGathererBehaviour : public iBehaviour
{
public:
	enum states
	{
		idle_base,
		finding_resource,
		traveling_to_resource,
		verify_resource_exists,
		gathering_resource,
		idle_resource,
		finding_base,
		traveling_to_base
	};

	float time_since_last_state_change;

	iBehaviour* active_behaviour;

	cScene* scene;
	states current_state;
	float base_wait;
	float resource_wait;

	cGathererBehaviour(float base_wait, float resource_wait, cScene* scene);
	void update(cEntity* entity, float delta_time) override;
	void set_active_behaviour(iBehaviour* behaviour);
};

