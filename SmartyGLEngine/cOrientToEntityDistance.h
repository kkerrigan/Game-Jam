

#pragma once
#include "iCommand.h"
#include "cEntity.h"

class cOrientToEntityDistance : public iCommand
{
	cEntity* entityToRotate;
	cEntity* entityTarget;

	bool firstUpdateDone;
	double initialTime;
	double elapsedTime;
	float duration;

	glm::vec3 initialPosition;
	glm::vec3 theUpVector;
	glm::vec3 lookAtPosition;
	glm::mat4 initialOrientation;
	glm::mat4 finalOrientation;

	float easeInDistance;
	float easeOutDistance;

	bool finished;

public:
	cOrientToEntityDistance(cEntity* entityToRotate, cEntity* entityTarget, float duration, float easeIn, float easeOut);

	void update(float deltaTime) override;
	bool isFinished() override;
	
};

