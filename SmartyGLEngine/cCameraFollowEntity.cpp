
#include "cCameraFollowEntity.h"
#include "cScriptUtil.h"
#include "cPhysicsComponent.h"

cCameraFollowEntity::cCameraFollowEntity(cFlyCamera* camera, cEntity* entityToFollow, glm::vec3 followDistance, float minDistance,
	float maxSpeedDistance, float maxSpeed, glm::vec3 offset, float easeInRange, float easeOutRange, float time) : camera(camera), entityToFollow(entityToFollow),
followDistance(followDistance),
offset(offset), minDistance(minDistance),
maxSpeed(maxSpeed),
maxSpeedDistance(maxSpeedDistance),
easeInRange(easeInRange),
easeOutRange(easeOutRange), orientToTarget(true),
finished(false), time(time), elapsedTime(0.0f)
{
}


void cCameraFollowEntity::update(float deltaTime)
{
	if(this->entityToFollow->components[8] == nullptr)
	{
		return;
	}

	this->elapsedTime += deltaTime;

	cPhysicsComponent* entityPhysics = static_cast<cPhysicsComponent*>(this->entityToFollow->components[8]);

	// Get the information from the object that is moving
	float currentDistance = glm::distance(entityPhysics->get_rigid_body()->get_position() + this->offset, this->camera->eye);

	/*if (this->elapsedTime >= this->time)
	{
		this->finished = true;
		return;
	}*/

	// Here, I am NOT where I'm supposed to be, so I need to move.
	// Calculate the direction vector to the target location
	glm::vec3 idealCameraLocation = entityPhysics->get_rigid_body()->get_position() + this->offset;


	// To calc velocity, I will normalize
	glm::vec3 vVel = glm::normalize(idealCameraLocation);

	// Adjust velocity in the "slow down" zone
	currentDistance = glm::distance(idealCameraLocation, this->camera->eye);

	float distance = cScriptUtil::clamp(this->minDistance, this->maxSpeedDistance, currentDistance);

	// Now distance will be MAX this->maxSpeedDistance...
	// Scale this from 0 to 1 (1 at max distance)
	float slowDownRatio = distance / (this->maxSpeedDistance - this->minDistance);

	// Smoothstep
	slowDownRatio = cScriptUtil::smoothstep_wikipedia(0.0f, 1.0f, slowDownRatio);

	//	std::cout << "slowDownRatio = " << slowDownRatio << std::endl;
	glm::vec3 direction = glm::normalize(entityPhysics->get_rigid_body()->get_position() + this->offset - this->camera->eye);


	if(distance > this->minDistance)
	{
		this->camera->eye += direction * slowDownRatio * this->maxSpeed * deltaTime;
	}
	
	if (this->orientToTarget)
	{
		glm::quat orient = (glm::lookAt(camera->eye - this->offset, entityPhysics->get_rigid_body()->get_position(),	//if hot garbage (glm::inverse x2)
			glm::vec3(0.0f,1.0f, 0.0f)));

		camera->setMeshOrientationQ((orient));
	}
}

bool cCameraFollowEntity::isFinished()
{
	return this->finished;
}
