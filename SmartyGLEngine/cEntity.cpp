#include "cEntity.h"
#include "cParticleComponent.h"


cEntity::cEntity(int type) : UNIQUE_ID(++nextUniqueId), componentBitField(0), entityType(type) {

	// initialize all the component pointers to null
	for (int i = 0; i != ENTITY_MAX_COMPONENTS; ++i) {
		this->components[i] = nullptr;
	}
}

cEntity::~cEntity() {}

void cEntity::serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {

	writer.StartObject();
	writer.String("id");
	writer.Int(this->UNIQUE_ID);

	writer.String("entityType");
	writer.Int(this->entityType);

	writer.String("componentBits");
	writer.Int(this->componentBitField);

	// Call serialize for each component
	writer.String(("components"));
	writer.StartArray();
	for (unsigned int i = 0; i < ENTITY_MAX_COMPONENTS; ++i) {
		if ((cEulerMovementComponent*)this->components[i] != nullptr) {

			switch (i) {
			case 1:
				((cTransformComponent*)(this->components[i]))->serializeComponent(writer);
				break;

			case 2:
				((cEulerMovementComponent*)(this->components[i]))->serializeComponent(writer);
				break;

			case 3:
				((cRenderMeshComponent*)(this->components[i]))->serializeComponent(writer);
				break;

			case 6:
				((c3dSoundComponent*)(this->components[i]))->serializeComponent(writer);
				break;

			case 7:
				((cParticleComponent*)(this->components[i]))->serializeComponent(writer);
				break;
			}
		}
	}
	writer.EndArray();
	writer.EndObject();

}

int cEntity::nextUniqueId = 1000;
