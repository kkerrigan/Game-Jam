#ifndef _cParticleSystem_HG_
#define _cParticleSystem_HG_

#include <vector>

#include "iSystem.h"

struct fire_info
{
	cEntity* smoke;
	cEntity* fire;
};

class cParticleSystem : public iSystem {
public:
	// Singleton setup
	static cParticleSystem* getInstance();
	cParticleSystem(cParticleSystem const&) = delete;
	void operator=(cParticleSystem const&) = delete;

	void update(double deltaTime);
	void registerEntity(cEntity* entity);
	void unregisterEntity(cEntity* entity);


private:
	std::vector<cEntity*> entitiesToUpdate;

	cParticleSystem();
	~cParticleSystem();
};

#endif // !_cParticleSystem_HG_

