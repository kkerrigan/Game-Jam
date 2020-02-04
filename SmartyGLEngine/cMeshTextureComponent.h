#pragma once

#include <vector>
#include "sTextureInfo.h"

#include "cComponent.h"

class cMeshTextureComponent : public cComponent {
public:
	cMeshTextureComponent() : cComponent(7) {}

	std::vector<sTextureInfo> textures;
};