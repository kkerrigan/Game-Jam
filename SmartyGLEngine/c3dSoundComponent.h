#ifndef _c3dSoundComponent_HG_
#define _c3dSoundComponent_HG_

#include "cComponent.h"
#include "cSoundManager.h"

class c3dSoundComponent : public cComponent {
public:
	cSoundManager::sSoundInfo* soundInfo;

	c3dSoundComponent(cSoundManager::sSoundInfo* info) : cComponent(6), soundInfo(info) {}

	void serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {
		writer.StartObject();

		writer.String("componentType");
		writer.Int(this->COMPONENT_TYPE_ID);
		
		this->soundInfo->serializeSound(writer);

		writer.EndObject();
	}

	~c3dSoundComponent() {
		delete this->soundInfo;
	}
};

#endif