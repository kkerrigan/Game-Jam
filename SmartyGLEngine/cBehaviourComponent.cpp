/*
* Name:		cBehaviourComponent.h
* Author:	Daniel Maclam
* Date		2019-03-03 5:56 PM
*/

#include "cBehaviourComponent.h"

void cBehaviourComponent::add_behaviour(iBehaviour* behaviour)
{
	this->behaviours.push_back(behaviour);
}

std::vector<iBehaviour*>& cBehaviourComponent::get_behaviours()
{
	return this->behaviours;
}