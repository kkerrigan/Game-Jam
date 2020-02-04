#pragma once
#include "cComponent.h"
#include <glm/glm.hpp>
#include "sLight.h"
#include <vector>

class cAttachedLightComponent : public cComponent {
public:

	struct sAttachedLight {
		glm::vec3 offsetFromOrigin;
		sLight* light;

		sAttachedLight(glm::vec3 offset, sLight* light) :offsetFromOrigin(offset), light(light){}
	};
	
	std::vector< sAttachedLight> attachedLights;

	cAttachedLightComponent() : cComponent(9) {}
	~cAttachedLightComponent() {};
};