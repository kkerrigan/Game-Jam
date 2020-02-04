/*
* Name:		cWaitBehaviour.h
* Author:	Daniel Maclam
* Date		2019-03-19 10:29 PM
*/

#include "cWaitBehaviour.h"

cWaitBehaviour::cWaitBehaviour(const float wait) : time_elapsed(0.0f)
{
	this->wait_time = wait;
}

void cWaitBehaviour::update(cEntity* entity, const float delta_time)
{
	this->time_elapsed +=delta_time;
}

bool cWaitBehaviour::is_finished() const
{
	return this->time_elapsed >= this->wait_time;
}
