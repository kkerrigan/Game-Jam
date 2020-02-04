#ifndef _cLightManager_HG_
#define _cLightManager_HG_

#include <vector>
#include "sLight.h"

#define MAX_NUM_OF_LIGHTS 15

class cLightManager {
public:
	static cLightManager* getInstance();
	cLightManager(cLightManager const&) = delete;
	void operator=(cLightManager const&) = delete;

	void initializeLights();
	void writeLightsToShader();

	sLight* getLightByIndex(int index) {
		return this->allocatedLights[index];
	}

private:
	cLightManager();
	~cLightManager();
	std::vector<sLight*> allocatedLights;

};

#endif
