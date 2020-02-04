
#include "cFollowEntity.h"
#include "cScriptUtil.h"

cFollowEntity::cFollowEntity(cEntity* entity, cEntity* entityToFollow, glm::vec3 followDistance, float minDistance,
                             float maxSpeedDistance, float maxSpeed, glm::vec3 offset,
							 float easeInRange, float easeOutRange): entity(entity), entityToFollow(entityToFollow),
							                                         followDistance(followDistance),
							                                         offset(offset), minDistance(minDistance),
							                                         maxSpeed(maxSpeed),
							                                         maxSpeedDistance(maxSpeedDistance),
							                                         easeInRange(easeInRange),
							                                         easeOutRange(easeOutRange), orientToTarget(true),
							                                         finished(false)
{
}

void cFollowEntity::update(float deltaTime)
{
	cTransformComponent* entityTransform = static_cast<cTransformComponent*>(this->entity->components[1]);
	cEulerMovementComponent* entityEuler = static_cast<cEulerMovementComponent*>(this->entity->components[2]);

	cTransformComponent* entityToFollowTransform = static_cast<cTransformComponent*>(this->entityToFollow->components[1]);

	// Get the information from the object that is moving
	float currentDistance = glm::distance(entityToFollowTransform->get_position() + this->offset, entityTransform->get_position());

	if (currentDistance <= this->minDistance)
	{
		entityEuler->velocity = glm::vec3(0.0f);
		this->finished = true;
		return;
	}

	glm::vec3 idealCameraLocation = entityToFollowTransform->get_position() + this->offset;
	glm::vec3 vVel = glm::normalize(idealCameraLocation);
	currentDistance = glm::distance(idealCameraLocation, entityTransform->get_position());

	float distance = cScriptUtil::clamp(this->minDistance, this->maxSpeedDistance, currentDistance);
	float slowDownRatio = distance / (this->maxSpeedDistance - this->minDistance);
	slowDownRatio = cScriptUtil::smoothstep_wikipedia(0.0f, 1.0f, slowDownRatio);

	glm::vec3 direction = glm::normalize(entityToFollowTransform->get_position() + this->offset - entityTransform->get_position());
	entityEuler->velocity = (direction * slowDownRatio * this->maxSpeed);

	if(this->orientToTarget)
	{
		entityTransform->setQOrientation(glm::inverse(glm::lookAt(
				entityTransform->get_position(), entityToFollowTransform->get_position() + this->offset,
				glm::vec3(0.0f,1.0f,0.0f))));
	}

}

bool cFollowEntity::isFinished()
{
	return this->finished;
}
