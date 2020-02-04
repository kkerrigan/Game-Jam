
#include "cOrientToEntityDistance.h"

cOrientToEntityDistance::cOrientToEntityDistance(cEntity* entityToRotate, cEntity* entityTarget, float duration,
	float easeIn, float easeOut) : entityToRotate(entityToRotate),
	entityTarget(entityTarget), firstUpdateDone(false),
	initialTime(0),
	elapsedTime(0), duration(duration),
	finished(false)
{
	initialPosition = static_cast<cTransformComponent*>(this->entityTarget->components[1])->get_position();
	lookAtPosition = initialPosition;
	theUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
	initialOrientation = glm::toMat4(static_cast<cTransformComponent*>(this->entityTarget->components[1])->getQOrientation());
	
	this->easeInDistance = easeIn;
	this->easeOutDistance = easeOut;

	this->finalOrientation = glm::inverse(glm::lookAt(
		static_cast<cTransformComponent*>(this->entityToRotate->components[1])->get_position(), this->lookAtPosition,
		this->theUpVector));
}

void cOrientToEntityDistance::update(float deltaTime)
{
	if (!this->firstUpdateDone)
	{
		this->initialTime = glfwGetTime();
		this->elapsedTime = 0.0f;

		initialPosition = static_cast<cTransformComponent*>(this->entityTarget->components[1])->get_position();
		lookAtPosition = initialPosition;
		theUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
		initialOrientation = glm::toMat4(static_cast<cTransformComponent*>(this->entityToRotate->components[1])->getQOrientation());

		this->finalOrientation = glm::inverse(glm::lookAt(
			static_cast<cTransformComponent*>(this->entityToRotate->components[1])->get_position(), this->lookAtPosition,
			this->theUpVector));

		this->firstUpdateDone = true;
	}

	cTransformComponent* entityTransform = static_cast<cTransformComponent*>(this->entityToRotate->components[1]);
	cTransformComponent* targetTransform = static_cast<cTransformComponent*>(this->entityTarget->components[1]);


	// Transform the Orientation Matrix to Quaternion
	glm::quat quatStart = glm::quat_cast(this->initialOrientation);
	glm::quat quatEnd = glm::quat_cast(this->finalOrientation);

	this->elapsedTime = glfwGetTime() - this->initialTime;
	float factor = this->elapsedTime / this->duration;

	//ease in / ease out
	//currently slerps so we have smoothing on each end

	// Do the orientation calculation over the Quaternion
	glm::quat quatInterp = glm::slerp(quatStart, quatEnd, factor);

	// Transform the resulting quaternion back to the Matrix
	static_cast<cTransformComponent*>(this->entityToRotate->components[1])->getQOrientation() = glm::quat(quatInterp);

	//are we done?
	if (glm::toMat4(quatInterp) == this->finalOrientation || this->elapsedTime >= this->duration) {
		this->finished = true;
	}
}

bool cOrientToEntityDistance::isFinished()
{
	return this->finished;
}
