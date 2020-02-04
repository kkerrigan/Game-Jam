
#pragma once
#include "iCommand.h"
#include <glm/vec3.hpp>
#include "cEntity.h"


class cOrientToEntity : public iCommand
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

	//ease in (time)
	float easeIn;
	float easeOut;

	bool finished;

public:
	cOrientToEntity(cEntity* entityToRotate, cEntity* entityTarget, float duration, float easeIn, float easeOut);

	void update(float deltaTime) override;
	bool isFinished() override;

};

