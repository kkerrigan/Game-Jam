#include "cBehaviourSystem.h"
#include "cBehaviourComponent.h"

void cBehaviourSystem::update(double deltaTime)
{
	for (size_t i = 0; i < this->entities_to_update.size(); ++i)
	{
		cBehaviourComponent* component = static_cast<cBehaviourComponent*>(this->entities_to_update[i]->components[13]);

		std::vector<iBehaviour*> behaviours = component->get_behaviours();

		for (size_t j = 0; j < behaviours.size(); ++j)
		{
			behaviours[j]->update(this->entities_to_update[i], deltaTime);
		}
	}

}

void cBehaviourSystem::registerEntity(cEntity* entity)
{
	this->entities_to_update.push_back(entity);
}

void cBehaviourSystem::unregisterEntity(cEntity* entity)
{
}