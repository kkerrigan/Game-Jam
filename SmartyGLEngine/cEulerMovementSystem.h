#ifndef _cEulerMovementSystem_HG_
#define _cEulerMovementSystem_HG_

#include <vector>

#include "cTransformComponent.h"
#include "cEulerMovementComponent.h"
#include "cEntity.h"


class cEulerMovementSystem {	
public:
	
	static cEulerMovementSystem* getInstance();
	cEulerMovementSystem(cEulerMovementSystem const&) = delete;
	void operator=(cEulerMovementSystem const&) = delete;

	void update(double deltaTime);
	void registerEntity(cEntity* entity);
	void unregisterEntity(cEntity* entity);

private:
	std::vector<cEntity*> entitiesToUpdate;

	cEulerMovementSystem();
	~cEulerMovementSystem();

};

#endif
