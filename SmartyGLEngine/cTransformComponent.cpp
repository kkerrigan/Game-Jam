#include "cTransformComponent.h"



cTransformComponent::cTransformComponent(): cComponent(1), position(glm::vec3(0.0f)), lastPosition(),
                                            orientation(glm::vec3(0.0f)),
                                            scale(glm::vec3(1.0f))
{
	InitializeCriticalSection(&position_critical_section);
	InitializeCriticalSection(&orientation_critical_section);
}

cTransformComponent::~cTransformComponent()
{
	DeleteCriticalSection(&position_critical_section);
	DeleteCriticalSection(&orientation_critical_section);
}

//Gems - Question #3 - CRITICAL SECTIONS TO PROTECT THE DATA
void cTransformComponent::set_position(glm::vec3 position)
{
	EnterCriticalSection( &position_critical_section );
	this->position = position;
	LeaveCriticalSection( &position_critical_section );
}

glm::vec3 cTransformComponent::get_position()
{
	EnterCriticalSection( &position_critical_section );
	glm::vec3 cur_position = this->position;
	LeaveCriticalSection( &position_critical_section );

	return cur_position;
}

void cTransformComponent::serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {
	writer.StartObject();

	writer.String("componentType");
	writer.Int(this->COMPONENT_TYPE_ID);

	writer.String("position");
	writer.StartObject();
	writer.String("x");
	writer.Double(this->position.x);
	writer.String("y");
	writer.Double(this->position.y);
	writer.String("z");
	writer.Double(this->position.z);
	writer.EndObject();

	//TODO: Update to serialize Quat
	glm::vec3 eulerOrientation = glm::degrees(glm::eulerAngles<float>(this->orientation));

	writer.String("orientation");
	writer.StartObject();
	writer.String("x");
	writer.Double(eulerOrientation.x);
	writer.String("y");
	writer.Double(eulerOrientation.y);
	writer.String("z");
	writer.Double(eulerOrientation.z);
	writer.EndObject();

	writer.String("scale");
	writer.StartObject();
	writer.String("x");
	writer.Double(this->scale.x);
	writer.String("y");
	writer.Double(this->scale.y);
	writer.String("z");
	writer.Double(this->scale.z);
	writer.EndObject();

	writer.EndObject();
}

//Gems - Question #3 - CRITICAL SECTIONS TO PROTECT THE DATA (extra for orientation)
glm::quat cTransformComponent::getQOrientation(void)
{
	EnterCriticalSection(&orientation_critical_section);
	glm::quat ori = this->orientation;
	LeaveCriticalSection(&orientation_critical_section);
	return ori;
};

void cTransformComponent::setOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees) {
	
	if (bIsDegrees) {
		newAnglesEuler = glm::vec3(glm::radians(newAnglesEuler.x), glm::radians(newAnglesEuler.y), glm::radians(newAnglesEuler.z));
	}

	this->orientation = glm::quat(glm::vec3(newAnglesEuler.x, newAnglesEuler.y, newAnglesEuler.z));
	return;
}

void cTransformComponent::setOrientationEulerAngles(float x, float y, float z, bool bIsDegrees) {
	this->setOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cTransformComponent::adjOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees) {
	
	if (bIsDegrees) {
		adjAngleEuler = glm::vec3(glm::radians(adjAngleEuler.x), glm::radians(adjAngleEuler.y), glm::radians(adjAngleEuler.z));
	}

	// Step 1: make a quaternion that represents the angle we want to rotate
	glm::quat rotationAdjust(adjAngleEuler);

	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
	this->orientation *= rotationAdjust;
}

void cTransformComponent::adjOrientationEulerAngles(float x, float y, float z, bool bIsDegrees) {
	this->adjOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cTransformComponent::adjMeshOrientationQ(glm::quat adjOrientQ) {
	this->orientation *= adjOrientQ;
}

void cTransformComponent::setQOrientation(glm::quat newOrientation)
{
	EnterCriticalSection(&orientation_critical_section);
	this->orientation = newOrientation;
	LeaveCriticalSection(&orientation_critical_section);
}
