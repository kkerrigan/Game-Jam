
#pragma once

#include "iCommand.h"
#include "cEntity.h"

#include <glm/vec3.hpp>

class cMoveToTime : public iCommand
{
	cEntity* entity;
	glm::vec3 destination;
	float time;
	float easeInTime;
	float easeOutTime;
	float commandRunTime;
	bool finished;

	float approxVelocityRequired;

	float findMaxVelocity();

public:
	cMoveToTime(cEntity* entity, glm::vec3 destination, float time, float easeInTime = 0.0f, float easeOutTime = 0.0f);
	void update(float deltaTime) override;
	bool isFinished() override;
};


