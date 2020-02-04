
#include "cMoveToTime.h"
#include "cTransformComponent.h"
#include "cScriptUtil.h"

float cMoveToTime::findMaxVelocity()
{
	//simulate the move so we can accurately in the defined time

	cTransformComponent* transformComponent = static_cast<cTransformComponent*>(this->entity->components[1]);

	//calculate the average velocity required to get to the destination in time
	const float distanceToDestination = glm::distance(this->destination, transformComponent->get_position());
	float avgVelocity = distanceToDestination / this->time;

	//add a bit for ease-in and ease-out
	avgVelocity *= 1.0f + (0.00355f * (this->easeInTime * this->easeOutTime) * (this->easeInTime * this->easeOutTime));

	return avgVelocity;
}

cMoveToTime::cMoveToTime(cEntity* entity, const glm::vec3 destination, const float time, const float easeInTime, const float easeOutTime) : approxVelocityRequired(0.0f)
{
	this->entity = entity;
	this->destination = destination;
	this->time = time;
	this->easeInTime = easeInTime;
	this->easeOutTime = easeOutTime;
	this->commandRunTime = 0.0f;
	this->finished = false;

	this->approxVelocityRequired = this->findMaxVelocity();
	

}

void cMoveToTime::update(float deltaTime)
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
	if(this->easeInTime > 0.0f && this->commandRunTime <= this->easeInTime)
	{
		avgVelocity *= cScriptUtil::smoothstep_wikipedia(0.1f, 1.0, this->commandRunTime/this->easeInTime);
	}
	else if (this->easeOutTime > 0.0f && this->commandRunTime >= (this->time- this->easeOutTime))
	{
		avgVelocity *= cScriptUtil::smoothstep_wikipedia(0.3f, 1.0f, (this->time - this->commandRunTime) / this->easeOutTime);
	}

	cTransformComponent* transform = static_cast<cTransformComponent*>(this->entity->components[1]);
	transform->set_position(transform->get_position() + direction * avgVelocity * deltaTime);

	//check if the entity reached its' destination or 
	//gone over its run time (this is needed as we "guess" the max velocity required) see findMaxVelocity())
	if(glm::abs(distanceToDestination) < 0.1f || this->commandRunTime >= this->time)
	{
		cEulerMovementComponent* euler = static_cast<cEulerMovementComponent*>(this->entity->components[2]);
		euler->velocity = glm::vec3(0.0f);
		this->finished = true;
	}
}

bool cMoveToTime::isFinished()
{
	return this->finished;
}
