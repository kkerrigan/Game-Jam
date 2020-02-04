

#include "cCameraMoveToDistance.h"

cCameraMoveToDistance::cCameraMoveToDistance(cFlyCamera* camera, glm::vec3 destination, float time, float easePercent,
	float easeOutPercent)
{
	this->camera = camera;
	this->destination = destination;
	this->time = time;
	this->easeInPercent = easePercent;
	this->easeOutPercent = easeOutPercent;
	this->commandRunTime = 0.0f;
	this->finished = false;
	this->distanceTraveled = 0.1f;
	this->distanceToTravel = glm::abs(glm::distance(this->camera->eye, this->destination));
	this->approxVelocityRequired = this->distanceToTravel / this->time;
}

void cCameraMoveToDistance::update(float deltaTime)
{
	//update the commands run time
	this->commandRunTime += deltaTime;
	const float timeLeft = this->time - this->commandRunTime;

	//calculate the average velocity required to get to the destination in time
	const float distanceToDestination = glm::distance(this->destination, this->camera->eye);
	const glm::vec3 direction = glm::normalize(this->destination - this->camera->eye);
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

	glm::vec3 deltaPosition = direction * avgVelocity * deltaTime;
	this->camera->eye += deltaPosition;

	this->distanceTraveled += glm::abs(distance(deltaPosition, glm::vec3(0.0f)));
}

bool cCameraMoveToDistance::isFinished()
{
	return this->finished;
}
