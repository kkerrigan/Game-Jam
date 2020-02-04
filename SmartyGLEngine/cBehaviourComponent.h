#pragma once
#include "cComponent.h"
#include "iBehaviour.h"
#include <vector>

/*!	\class	cBehaviourComponent
	\brief
	\author Daniel Maclam
	\date	2019-03-03 5:56 PM
	*/
class cBehaviourComponent : public cComponent
{
	std::vector<iBehaviour*> behaviours;
public:
	cBehaviourComponent() : cComponent(13) {}
	void add_behaviour(iBehaviour* behaviour);
	std::vector<iBehaviour*>& get_behaviours();
};