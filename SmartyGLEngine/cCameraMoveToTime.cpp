

#include "cCameraMoveToTime.h"
#include "cScriptUtil.h"

cCameraMoveToTime::cCameraMoveToTime(cFlyCamera* entity, glm::vec3 destination, float time, float easeInTime,
	float easeOutTime)
{
	this->camera = entity;
	this->destination = destination;
	this->time = time;
	this->easeInTime = easeInTime;
	this->easeOutTime = easeOutTime;
	this->commandRunTime = 0.0f;
	this->finished = false;

	this->approxVelocityRequired = this->findMaxVelocity();
}

float cCameraMoveToTime::findMaxVelocity()
{

	//calculate the average velocity required to get to the destination in time
	const float distanceToDestination = glm::distance(this->destination, this->camera->eye);
	float avgVelocity = distanceToDestination / this->time;

	//add a bit for ease-in and ease-out
	avgVelocity *= 1.0f + (0.00355f * (this->easeInTime * this->easeOutTime) * (this->easeInTime * this->easeOutTime));

	return avgVelocity;
}

void cCameraMoveToTime::update(float deltaTime)
{
	//update the commands run time
	this->commandRunTime += deltaTime;
	const float timeLeft = this->time - this->commandRunTime;

	//calculate the average velocity required to get to the destination in time
	const float distanceToDestination = glm::distance(this->destination, camera->eye);
	const glm::vec3 direction = glm::normalize(this->destination - camera->eye);
	float avgVelocity = this->approxVelocityRequired;

	//ease in/out
	if (this->easeInTime > 0.0f && this->commandRunTime <= this->easeInTime)
	{
		avgVelocity *= cScriptUtil::smoothstep_wikipedia(0.1f, 1.0, this->commandRunTime / this->easeInTime);
	}
	else if (this->easeOutTime > 0.0f && this->commandRunTime >= (this->time - this->easeOutTime))
	{
		avgVelocity *= cScriptUtil::smoothstep_wikipedia(0.3f, 1.0f, (this->time - this->commandRunTime) / this->easeOutTime);
	}

	camera->eye += direction * avgVelocity * deltaTime;

	//check if the entity reached its' destination or 
	//gone over its run time (this is needed as we "guess" the max velocity required) see findMaxVelocity())
	if (glm::abs(distanceToDestination) < 0.1f || this->commandRunTime >= this->time)
	{
		this->finished = true;
	}
}

bool cCameraMoveToTime::isFinished()
{
	return this->finished;
}
