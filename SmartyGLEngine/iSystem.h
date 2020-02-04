#ifndef _iSystem_HG_
#define _iSystem_HG_

#include "cEntity.h"

class iSystem {

public:
	virtual void update(double deltaTime) = 0;
	virtual void registerEntity(cEntity* entity) = 0;
	virtual void unregisterEntity(cEntity* entity) = 0;


	virtual ~iSystem() {}
};

#endif
