
#pragma once
#include "iSystem.h"
#include "cCommandGroup.h"

class cScriptingCommandSystem : public iSystem
{
private:
	cCommandGroup* masterCommandGroup;

	cScriptingCommandSystem();
	~cScriptingCommandSystem();

public:
	static cScriptingCommandSystem* getInstance()
	{
		static cScriptingCommandSystem instance;
		return &instance;
	}

	void update(double deltaTime) override;
	void registerEntity(cEntity* entity) override;
	void unregisterEntity(cEntity* entity) override;

	void setMasterCommandGroup(cCommandGroup* group);
};

