
#include "cMoveToDistance.h"
#include "cTransformComponent.h"
#include "cEulerMovementComponent.h"
#include "cScriptUtil.h"
#include "cEntity.h"

cMoveToDistance::cMoveToDistance(cEntity* entity, glm::vec3 destination, float time, float easePercent,
	float easeOutPercent)
{
	this->entity = entity;
	this->destination = destination;
	this->time = time;
	this->easeInPercent = easePercent;
	this->easeOutPercent = easeOutPercent;
	this->commandRunTime = 0.0f;
	this->finished = false;
	this->distanceTraveled = 0.1f;
	this->distanceToTravel = glm::abs(glm::distance(static_cast<cTransformComponent*>(this->entity->components[1])->get_position(), this->destination));
	this->approxVelocityRequired = this->distanceToTravel / this->time;

}

void cMoveToDistance::update(float deltaTime)
{
	//update the commands run time
	this->commandRunTime += deltaTime;
	const float timeLeft = this->time - this->commandRunTime;

	cTransformComponent* transformComponent = static_cast<cTransformComponent*>(this->entity->components[1]);

	//calculate the average velocity required to get to the destination in time
	const float distanceToDestination = glm::distance(this->destination, transformComponent->get_position());
	const glm::vec3 direction = glm::normalize(this->destination - transformComponent->get_position());
	float avgVelocity = this->approxVelocityRequired;

	//ease in/out
	if (this->easeInPercent > 0.0f && this->distanceTraveled / this->distanceToTravel <= this->easeInPercent)
	{
		avgVelocity *= glm::mix(0.1f, 1.0f, (this->distanceTraveled / this->distanceToTravel) / this->easeInPercent);
	}
	else if (this->easeOutPercent > 0.0f && (1.0f - (this->distanceTraveled / this->distanceToTravel)) <= this->easeOutPercent)
	{
		avgVelocity *= glm::mix(0.1f, 1.0f, (1.0f - (this->distanceTraveled / this->distanceToTravel)) / this->easeOutPercent);
	}

	cTransformComponent* transform = static_cast<cTransformComponent*>(this->entity->components[1]);
	glm::vec3 deltaPosition = direction * avgVelocity * deltaTime;
	transform->set_position(transform->get_position() + deltaPosition);

	this->distanceTraveled += glm::abs(distance(deltaPosition, glm::vec3(0.0f)));

	//check if the entity reached its' destination or 
	//gone over its run time (this is needed as we "guess" the max velocity required) see findMaxVelocity())
	if (glm::abs(distanceToDestination) < 0.1f)
	{
		cEulerMovementComponent* euler = static_cast<cEulerMovementComponent*>(this->entity->components[2]);
		euler->velocity = glm::vec3(0.0f);
		this->finished = true;
	}
}

bool cMoveToDistance::isFinished()
{
	return this->finished;
}
