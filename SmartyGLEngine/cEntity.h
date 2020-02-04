#ifndef _cEntity_HG_
#define _cEntity_HG_

#include "cEulerMovementComponent.h"
#include "cRenderMeshComponent.h"
#include "cTransformComponent.h"
#include "c3dSoundComponent.h"

#include <rapidjson/prettywriter.h>

#define ENTITY_MAX_COMPONENTS 64 // we have an int as our component bit field

class cEntity {	
public:
	int UNIQUE_ID;
	const int entityType;
	int componentBitField;
	void* components[ENTITY_MAX_COMPONENTS];

	void serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

	cEntity(int type);
	~cEntity();

private:
	static int nextUniqueId;

};

#endif
