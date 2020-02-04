
#pragma once
#include "iCommand.h"
#include <glm/vec3.hpp>

class cEntity;

class cMoveToDistance : public iCommand
{
	cEntity* entity;
	glm::vec3 destination;
	float time;
	float easeInPercent;
	float easeOutPercent;
	float commandRunTime;
	bool finished;

	float distanceToTravel;
	float distanceTraveled;

	float approxVelocityRequired;

	//float findMaxVelocity();

public:
	cMoveToDistance(cEntity* entity, glm::vec3 destination, float time, float easePercent = 0.0f, float easeOutPercent = 0.0f);
	void update(float deltaTime) override;
	bool isFinished() override;
};

