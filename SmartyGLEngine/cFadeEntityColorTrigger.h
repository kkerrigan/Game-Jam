
#pragma once
#include "cEntity.h"
#include "iCommand.h"

class cFadeEntityColorTrigger : public iCommand
{
public:
	cEntity* entity;
	glm::vec4 origColor;
	glm::vec4 destColor;
	float fadeDuration;

	double lastFrameTime;
	float elapsedTime;
	bool finished;

	float radius;
	bool isTriggered;

	cFadeEntityColorTrigger(cEntity* entity, glm::vec4 color, float fadeDuration, float radius);

	void update(float deltaTime) override;
	bool isFinished() override;
};

