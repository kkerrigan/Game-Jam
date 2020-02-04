
#include "cFollowCurve.h"
#include "cTransformComponent.h"

cFollowCurve::cFollowCurve(cEntity* entity, glm::vec3 destination, float time, glm::vec3 controlPoint, float easeIn, float easeOut)
{
	this->entity = entity;

	this->startPosition = static_cast<cTransformComponent*>(this->entity->components[1])->get_position();
	this->lastPosition = static_cast<cTransformComponent*>(this->entity->components[1])->get_position();

	this->traveledDistance = 0.0f;

	this->velocity = 0.0f;
	this->initialTime = 0;
	this->elapsedTime = 0;
	this->hasStarted = false;
	this->finished = false;

	this->finalPosition = destination;
	this->direction = glm::normalize(finalPosition - startPosition);
	this->distanceToTarget = glm::distance(finalPosition, startPosition);
	this->duration = time;
	this->controlPoint = controlPoint;

	this->easeInDistance = easeIn;
	this->easeOutDistance = easeOut;

	this->curveDistance = this->calculateCurveLength(this->startPosition, this->controlPoint, this->finalPosition);

	// This is the average velocity it would take to reach the destination
	this->velocity = curveDistance / time + ((this->easeInDistance + this->easeOutDistance) * 0.5);
}

void cFollowCurve::update(float deltaTime)
{
	cTransformComponent* transformComponent = static_cast<cTransformComponent*>(this->entity->components[1]);

	// Start the time 
	if (this->initialTime == 0.0f)
	{
		this->initialTime = glfwGetTime();
	}

	if (!this->hasStarted)
	{
		this->hasStarted = true;
		this->startPosition = transformComponent->get_position();
		this->lastPosition = this->startPosition;
	}

	this->elapsedTime = glfwGetTime() - this->initialTime;

	float ratio = 0.0f;

	if (transformComponent->get_position() != this->lastPosition)
	{
		this->traveledDistance += glm::distance(transformComponent->get_position(), this->lastPosition);
	}

	// Update the previous position
	this->lastPosition = transformComponent->get_position();

	glm::vec3 point1 = this->startPosition;
	glm::vec3 point2 = this->controlPoint;
	glm::vec3 point3 = this->finalPosition;

	float completionPercent = this->elapsedTime / this->duration;

	float i = 0.0f;
	if (completionPercent > 1.0f)
	{
		i = 1.0f;
	}
	else
	{
		i = completionPercent;
	}

	float factor = glm::smoothstep(0.1f, this->easeInDistance, this->elapsedTime)
		* (1 - glm::smoothstep(this->easeOutDistance, this->duration, this->elapsedTime));

	if (factor != 0.0f)
		i = glm::pow(completionPercent, factor);
	else
		i = completionPercent;

	if (i > 1.0f)
	{
		i = 1.0f;
	}
	else if (i < 0.0f)
	{
		i = 0.0f;
	}
		
	// Calculating the position of the line
	float xa = this->calculatePointOnCurve(point1.x, point2.x, i);
	float ya = this->calculatePointOnCurve(point1.y, point2.y, i);
	float za = this->calculatePointOnCurve(point1.z, point2.z, i);

	float xb = this->calculatePointOnCurve(point2.x, point3.x, i);
	float yb = this->calculatePointOnCurve(point2.y, point3.y, i);
	float zb = this->calculatePointOnCurve(point2.z, point3.z, i);

	// Calculate the point over the line position (position on the curve)
	float x = this->calculatePointOnCurve(xa, xb, i);
	float y = this->calculatePointOnCurve(ya, yb, i);
	float z = this->calculatePointOnCurve(za, zb, i);

	//orient the object to face the direction it is moving
	glm::vec3 direction = glm::normalize(glm::vec3(x, y, z) - transformComponent->get_position());

	if(glm::distance(glm::vec3(0.0f), direction) > 0.5f)
	{
		transformComponent->setQOrientation(quat_cast(glm::inverse(glm::lookAt(
			static_cast<cTransformComponent*>(this->entity->components[1])->get_position(), static_cast<cTransformComponent*>(this->entity->components[1])->get_position() + direction,
			glm::vec3(0.0f, 1.0f, 0.0f)))));
	}

	transformComponent->set_position(glm::vec3(x,y,z));
	
	if (transformComponent->get_position() == this->finalPosition ||
		this->elapsedTime >= this->duration)
	{
		this->finished = true;
	}
}

bool cFollowCurve::isFinished()
{
	return this->finished;
}

//quadratic bezier curve (length) 
float cFollowCurve::calculateCurveLength(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
{
	std::vector<glm::vec3> initialPoints;
	initialPoints.push_back(p0);
	initialPoints.push_back(p1);
	initialPoints.push_back(p2);

	std::vector< glm::vec3 > segments;

	glm::vec3 segment1 = initialPoints[1] - initialPoints[0];
	segments.push_back(segment1);

	glm::vec3 segment2 = initialPoints[0] - 2.0f * initialPoints[1] + initialPoints[2];
	segments.push_back(segment2);

	float length;

	if (segments[1] != glm::vec3(0.0f))
	{
		// Coefficients of f(t) = c*t^2 + b*t + a.

		float c = 4.0f * glm::dot(segments[1], segments[1]);  // c > 0 to be in this block of code
		float b = 8.0f * glm::dot(segments[0], segments[1]);
		float a = 4.0f * glm::dot(segments[0], segments[0]);  // a > 0 by assumption
		float q = 4.0f * a * c - b * b;  // = 16*|Cross(A0,A1)| >= 0

		float twoCpB = 2.0f * c + b;
		float sumCBA = c + b + a;
		float mult0 = 0.25f / c;
		float mult1 = q / (8.0f * powf(c, 1.5f));
		length =
			mult0 * (twoCpB * sqrtf(sumCBA) - b * sqrtf(a)) +
			mult1 * (logf(2.0f * sqrtf(c * sumCBA) + twoCpB) - logf(2.0f * sqrtf(c * a) + b));
	}
	else
	{
		length = 2.0f * glm::length(initialPoints[1] - initialPoints[0]);
	}

	return length;
}

float cFollowCurve::calculatePointOnCurve(float min, float max, float percent)
{
	float diff = max - min;

	return min + (diff * percent);
}
