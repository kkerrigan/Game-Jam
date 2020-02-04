

#pragma once
#include "iCommand.h"
#include "cEntity.h"
#include "cFlyCamera.h"

class cCameraFollowEntity : public iCommand
{

	float elapsedTime;

public:
	cFlyCamera* camera;
	cEntity* entityToFollow;
	glm::vec3 followDistance;
	glm::vec3 offset;
	float minDistance;
	float maxSpeed;
	float maxSpeedDistance;
	float easeInRange;
	float easeOutRange;
	bool orientToTarget;
	bool finished;
	float time;

	cCameraFollowEntity(cFlyCamera* camera, cEntity* entityToFollow, glm::vec3 followDistance, float minDistance,
		float maxSpeedDistance, float maxSpeed, glm::vec3 offset, float easeInRange, float easeOutRange, float time);
	void update(float deltaTime) override;
	bool isFinished() override;
	
};

