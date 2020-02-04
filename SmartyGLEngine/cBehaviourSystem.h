#pragma once

#include <vector>
#include "cEntity.h"
#include "iSystem.h"

/*!	\class	cBehaviourSystem
	\brief
	\author Daniel Maclam
	\date	2019-03-03 5:48 PM
	*/
class cBehaviourSystem : public iSystem
{
public:
	static cBehaviourSystem* get_instance()
	{
		static cBehaviourSystem instance;
		return &instance;
	}

	void update(double deltaTime) override;
	void registerEntity(cEntity* entity) override;
	void unregisterEntity(cEntity* entity) override;
private:
	cBehaviourSystem() {}
	~cBehaviourSystem() {}

	std::vector<cEntity*> entities_to_update;
};




