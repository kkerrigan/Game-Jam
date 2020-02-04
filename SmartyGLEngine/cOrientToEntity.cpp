

#include "cOrientToEntity.h"
#include "cTransformComponent.h"

cOrientToEntity::cOrientToEntity(cEntity* entityToRotate, cEntity* entityTarget, float duration, float easeIn, float easeOut):
	entityToRotate(entityToRotate),
	entityTarget(entityTarget), firstUpdateDone(false),
	initialTime(0),
	elapsedTime(0), duration(duration),
	finished(false)
{
	initialPosition = static_cast<cTransformComponent*>(this->entityTarget->components[1])->get_position();
	lookAtPosition = initialPosition;
	theUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
	initialOrientation = glm::toMat4(static_cast<cTransformComponent*>(this->entityTarget->components[1])->getQOrientation());

	this->easeIn = easeIn;
	this->easeOut = easeOut;

	this->finalOrientation = glm::inverse(glm::lookAt(
		static_cast<cTransformComponent*>(this->entityToRotate->components[1])->get_position(), this->lookAtPosition,
		this->theUpVector));
}

void cOrientToEntity::update(float deltaTime)
{
	if(!this->firstUpdateDone)
	{
		this->initialTime = glfwGetTime();
		this->elapsedTime = 0.0f;

		initialPosition = static_cast<cTransformComponent*>(this->entityTarget->components[1])->get_position();
		lookAtPosition = initialPosition;
		theUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
		initialOrientation = glm::toMat4(static_cast<cTransformComponent*>(this->entityToRotate->components[1])->getQOrientation());

		this->finalOrientation = (glm::lookAt(
			static_cast<cTransformComponent*>(this->entityToRotate->components[1])->get_position(), this->lookAtPosition,
			this->theUpVector));

		this->firstUpdateDone = true;
	}

	cTransformComponent* entityTransform = static_cast<cTransformComponent*>(this->entityToRotate->components[1]);
	cTransformComponent* targetTransform = static_cast<cTransformComponent*>(this->entityTarget->components[1]);

	glm::quat quatStart = glm::quat_cast(this->initialOrientation);
	glm::quat quatEnd = glm::quat_cast(this->finalOrientation);

	this->elapsedTime = glfwGetTime() - this->initialTime;
	float factor = this->elapsedTime / this->duration;

	//ease in / ease out
	float easeInFactor = 1.0f;
	if (this->easeIn > 0.0f && this->elapsedTime <= this->easeIn)
	{
		easeInFactor = this->elapsedTime / this->easeIn;
	}

	float easeOutFactor = 1.0f;
	if (this->easeOut > 0.0f && (this->duration - this->easeOut) < this->elapsedTime)
	{
		easeOutFactor = (this->elapsedTime / this->duration);
	}

	//interpolate the rotation between the start and end rotation
	glm::quat quatInterp = glm::slerp(quatStart, quatEnd, factor * easeInFactor * easeOutFactor);

	// Transform the resulting quaternion back to the Matrix
	static_cast<cTransformComponent*>(this->entityToRotate->components[1])->getQOrientation() = glm::quat(quatInterp);

	if(glm::toMat4(quatInterp) == this->finalOrientation || this->elapsedTime >= this->duration){	
		this->finished = true;
	}

	return;
}

bool cOrientToEntity::isFinished()
{
	return this->finished;
}
