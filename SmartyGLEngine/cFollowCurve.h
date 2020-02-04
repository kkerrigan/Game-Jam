
#pragma once
#include "iCommand.h"

#include <vector>
#include <glm/vec3.hpp>
#include "cEntity.h"

class cFollowCurve : public iCommand
{
public:
	cEntity* entity;

	glm::vec3 startPosition;
	glm::vec3 lastPosition;
	glm::vec3 finalPosition;

	glm::vec3 controlPoint;
	glm::vec3 direction;

	float traveledDistance;
	float distanceToTarget;
	float velocity;
	double initialTime;
	float duration;
	float elapsedTime;
	float curveDistance;

	float easeInDistance;
	float easeOutDistance;

	bool hasStarted;
	bool finished;

	cFollowCurve(cEntity* entity, glm::vec3 destination, float time, glm::vec3 controlPoint, float easeIn, float easeOut);
	void update(float deltaTime) override;
	bool isFinished() override;

private:
	float calculateCurveLength(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
	float calculatePointOnCurve(float min, float max, float percent);
};

