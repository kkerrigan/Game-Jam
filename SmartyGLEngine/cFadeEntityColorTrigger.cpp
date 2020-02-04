

#include "cFadeEntityColorTrigger.h"
#include "cSceneManager.h"

cFadeEntityColorTrigger::cFadeEntityColorTrigger(cEntity* entity, glm::vec4 color, float fadeDuration, float radius) :
	finished(false), isTriggered(false)
{
	this->entity = entity;
	this->origColor = static_cast<cRenderMeshComponent*>(this->entity->components[3])->objColour;
	this->destColor = color;
	this->fadeDuration = fadeDuration;
	this->radius = radius;
	this->elapsedTime = 0.0f;
}

void cFadeEntityColorTrigger::update(float deltaTime)
{
	if(this->isTriggered)
	{
		this->elapsedTime += deltaTime;

		static_cast<cRenderMeshComponent*>(this->entity->components[3])->objColour = glm::mix(this->origColor, this->destColor, this->elapsedTime / this->fadeDuration);


		if(elapsedTime > this->fadeDuration)
		{
			this->isTriggered = false;
			this->elapsedTime = 0.0f;
		}

		return;
	}

	cTransformComponent* triggerTransform = static_cast<cTransformComponent*>(this->entity->components[1]);
	glm::vec3 triggerPosition = triggerTransform->get_position();
	float radius = this->radius;

	//check for trigger hit
	for (std::vector<cEntity*>::iterator it = cSceneManager::getInstance()->getActiveScene()->getEntities().begin(); it != cSceneManager::getInstance()->getActiveScene()->getEntities().end(); ++it)
	{
		if(*it == this->entity)
		{
			continue;
		}

		float distance = glm::distance(static_cast<cTransformComponent*>((*it)->components[1])->get_position(),triggerTransform->get_position());
		if(distance <= 1.0f)
		{
			this->isTriggered = true;
		}

	}
}

bool cFadeEntityColorTrigger::isFinished()
{
	return this->finished;
}
