#pragma once

#include <vector>
#include "cEntity.h"
#include "iSystem.h"

class cDebugRenderSystem : public iSystem {
private:
	std::vector<cEntity*> entitiesToUpdate;

	cDebugRenderSystem() {};
	~cDebugRenderSystem() {};

public:

	static cDebugRenderSystem* getInstance();
	cDebugRenderSystem(cDebugRenderSystem const&) = delete;
	void operator=(cDebugRenderSystem const&) = delete;

	void update(double deltaTime);
	void registerEntity(cEntity* entity);
	void unregisterEntity(cEntity* entity);
};
