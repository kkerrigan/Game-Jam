
#include "cScriptingCommandSystem.h"

cScriptingCommandSystem::cScriptingCommandSystem() : masterCommandGroup(nullptr)
{
}

cScriptingCommandSystem::~cScriptingCommandSystem()
{
}

void cScriptingCommandSystem::update(double deltaTime)
{
	if(this->masterCommandGroup != nullptr)
	{
		this->masterCommandGroup->update(static_cast<float>(deltaTime));
	}
}

void cScriptingCommandSystem::registerEntity(cEntity* entity)
{
}

void cScriptingCommandSystem::unregisterEntity(cEntity* entity)
{
}

void cScriptingCommandSystem::setMasterCommandGroup(cCommandGroup* group)
{
	this->masterCommandGroup = group;
}
