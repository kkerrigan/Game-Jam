

#pragma once
#include "iCommand.h"
#include <glm/vec3.hpp>
#include "cFlyCamera.h"

class cCameraMoveToTime : public iCommand
{
public:
	cFlyCamera* camera;
	glm::vec3 destination;
	float time;
	float easeInTime;
	float easeOutTime;
	float commandRunTime;
	bool finished;

	float approxVelocityRequired;

	cCameraMoveToTime(cFlyCamera* entity, glm::vec3 destination, float time, float easeInTime = 0.0f, float easeOutTime = 0.0f);

	float findMaxVelocity();

	void update(float deltaTime) override;
	bool isFinished() override;
};

