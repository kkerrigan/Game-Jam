#include "cFlyCamera.h"
#include "cPhysicsComponent.h"

cFlyCamera::cFlyCamera() {
	this->eye = glm::vec3(0.0f);

	this->minFollowDistance = 20.0f;
	this->cameraMoveSpeed = 30.0f;

	// This will be constant
	this->m_frontOfCamera = glm::vec3(0.0f, 0.0f, 1.0f);
	this->m_upIsYVector = glm::vec3(0.0f, 1.0f, 0.0f);
	this->movementSpeed = 1.0f;

	// Set initial orientation (all zero on Euler axes)
	this->qOrientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	// If we are using the mouse
	this->setMouseXY(0.0, 0.0);
	this->m_MouseWheel = 0.0f;
	this->m_Mouse_Initial_State_Is_Set = false;

	return;
}

glm::vec3 cFlyCamera::getAtInWorldSpace(void) {
	// The "At" is relative to the where the camera is...
	// So just add the "At" to the current camera location
	return this->eye + this->m_at;
}

glm::vec3 cFlyCamera::getCameraDirection(void) {
	this->m_UpdateAtFromOrientation();

	return this->m_at;
}

void cFlyCamera::setMeshOrientationQ(glm::quat adjOrientQ) {
	
	this->qOrientation = adjOrientQ;
	this->m_UpdateAtFromOrientation();

	return;
}

void cFlyCamera::MoveForward_Z(float amount) {
	// We take the vector from the at to eye (i.e. direction)
	// Add this forward velocity along this direction.

	glm::vec3 direction = this->getAtInWorldSpace() - this->eye;

	// Make direction a "unit length"
	direction = glm::normalize(direction);

	// Generate a "forward" adjustment value 
	glm::vec3 amountToMove = direction * amount;

	// Add this to the eye
	this->eye += amountToMove;

	return;
}

void cFlyCamera::MoveUpDown_Y(float amount) {
	// We use the Up vector to determine which direction (in world space) we need to go

	glm::vec3 vecAmountToMove = glm::normalize(this->getUpVector()) * amount;

	this->eye += vecAmountToMove;

	return;
}

void cFlyCamera::MoveLeftRight_X(float amount) {
	// left and right is a little tricky, as we can "roll", 
	// because left and right change in world space.
	 
	// If you perform a cross product between the "forward" and "up" vectors, you'll
	//	get a vector perpendicular to both of these, so left and right.
	// Example: Assuming "forward" is the Z axis, and "up" is the Y axis, then 
	//	performing the cross product of these gives you the X axis. 

	glm::vec3 vecLeft = glm::cross(this->getCameraDirection(), this->getUpVector());

	glm::vec3 vecAmountToMove = glm::normalize(vecLeft) * amount;

	this->eye += vecAmountToMove;

	return;
}


void cFlyCamera::Pitch_UpDown(float angleDegrees) {
	// Adjust the orientation of the camera by the number of degrees
	this->adjMeshOrientationEulerAngles(glm::vec3(angleDegrees, 0.0f, 0.0f), true);

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	return;
}

void cFlyCamera::Yaw_LeftRight(float angleDegrees) {
	// Adjust the orientation of the camera by the number of degrees
	this->adjMeshOrientationEulerAngles(glm::vec3(0.0f, angleDegrees, 0.0f), true);

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	return;
}

void cFlyCamera::Roll_CW_CCW(float angleDegrees) {
	// Adjust the orientation of the camera by the number of degrees
	this->adjMeshOrientationEulerAngles(glm::vec3(0.0f, 0.0f, angleDegrees), true);

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();

	return;
}

void cFlyCamera::setMeshOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees /*=false*/) {
	
	if (bIsDegrees) {
		newAnglesEuler = glm::vec3(glm::radians(newAnglesEuler.x),
			glm::radians(newAnglesEuler.y),
			glm::radians(newAnglesEuler.z));
	}

	this->qOrientation = glm::quat(glm::vec3(newAnglesEuler.x, newAnglesEuler.y, newAnglesEuler.z));
	return;
}

void cFlyCamera::setMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees /*=false*/) {
	return this->setMeshOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cFlyCamera::adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees /*=false*/) {
	
	if (bIsDegrees) {
		adjAngleEuler = glm::vec3(glm::radians(adjAngleEuler.x),
			glm::radians(adjAngleEuler.y),
			glm::radians(adjAngleEuler.z));
	}

	// Step 1: make a quaternion that represents the angle we want to rotate
	glm::quat rotationAdjust(adjAngleEuler);

	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
	this->qOrientation *= rotationAdjust;
	this->qOrientation = this->qOrientation * rotationAdjust;

	return;
}

void cFlyCamera::adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees /*=false*/) {
	return this->adjMeshOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cFlyCamera::adjMeshOrientationQ(glm::quat adjOrientQ) {
	
	this->qOrientation *= adjOrientQ;

	this->m_UpdateAtFromOrientation();
	this->m_UpdateUpFromOrientation();
	
	return;
}

void cFlyCamera::m_UpdateAtFromOrientation(void) {
	// Have a rotation around the origin (eye)
	// vec4 new = mat4Transform * (vec4)Location
	// c'tor of mat4 can take a quaternion and generate 
	//	a rotation matrix from it... 
	glm::mat4 matRotation = glm::mat4(this->qOrientation);

	// Need to make these 4x4
	// NOTE: You could just keep these as 3x3 matrix values
	// (so you could keep them as vec3's....)

	glm::vec4 frontOfCamera = glm::vec4(this->m_frontOfCamera, 1.0f);

	glm::vec4 newAt = matRotation * frontOfCamera;

	// Update the "At"
	this->m_at = glm::vec3(newAt);

	return;
}

void cFlyCamera::m_UpdateUpFromOrientation(void) {
	glm::mat4 matRotation = glm::mat4(this->qOrientation);

	glm::vec4 upVector = glm::vec4(this->m_upIsYVector, 1.0f);

	glm::vec4 newUp = matRotation * upVector;

	// Update the "At"
	this->m_up = glm::vec3(newUp);

	return;
}

glm::vec3 cFlyCamera::getUpVector(void) {
	return this->m_up;
}

void cFlyCamera::setMouseXY(double newX, double newY) {
	this->m_lastMouse_X = this->m_Mouse_X;
	this->m_Mouse_X = (float)newX;

	this->m_lastMouse_Y = this->m_Mouse_Y;
	this->m_Mouse_Y = (float)newY;

	// Can return value numbers
	this->m_Mouse_Initial_State_Is_Set = true;

	return;
}

void cFlyCamera::setMouseWheelDelta(double deltaWheel) {
	this->m_lastMouse_Wheel = this->m_MouseWheel;
	this->m_MouseWheel += (float)deltaWheel;
	return;
}

float cFlyCamera::getMouseX(void) {
	return this->m_Mouse_X;
}

float cFlyCamera::getMouseY(void) {
	return this->m_Mouse_Y;
}

float cFlyCamera::getDeltaMouseX(void) {
	
	if (!this->m_Mouse_Initial_State_Is_Set) {
		return 0.0f;
	}
	return this->m_Mouse_X - this->m_lastMouse_X;
}

float cFlyCamera::getDeltaMouseY(void) {
	
	if (!this->m_Mouse_Initial_State_Is_Set) {
		return 0.0f;
	}
	return this->m_Mouse_Y - this->m_lastMouse_Y;
}

float cFlyCamera::getMouseWheel(void) {
	return this->m_MouseWheel;
}

void cFlyCamera::followSelectedPhysicsEntity(float deltaTime, cEntity* pEntity) {

	//// Check to see that the entity has a physics component
	//cPhysicsComponent* physicsComp = static_cast<cPhysicsComponent*>(pEntity->components[4]);
	//if (!physicsComp) return;

	//// Get the rigid body and necessary positions
	//physics::interfaces::iRigidBody* pBody = physicsComp->getRigidBody();
	//glm::vec3 currPos;
	//pBody->getPosition(currPos);
	//glm::vec3 currCamPos = this->eye;

	//// Calculate distance and direction
	//float distance = glm::distance(currPos, currCamPos);
	//glm::vec3 direction = glm::normalize(currPos - currCamPos);
	//glm::vec3 destination = currPos + (glm::vec3(0.0f, 5.0f, -5.0f) * direction);
	//glm::vec3 cameraTargetDirection = glm::normalize(destination - currCamPos);

	//// TODO: Zoom in/out

	//// Move towards entity
	//if ((distance + 5.0f > this->minFollowDistance) && (distance - 5.0f < this->minFollowDistance)) {
	//	 // close enough do nothing
	//}
	//else if (distance > this->minFollowDistance) { // Move towards entity

	//	this->eye += cameraTargetDirection * this->cameraMoveSpeed * deltaTime;
	//}
	//else if (distance < this->minFollowDistance) { // Move away from the entity

	//	this->eye += cameraTargetDirection * (-this->cameraMoveSpeed) * deltaTime;
	//}

	//// Keep the camera a certain height above the entity
	//this->eye.y = currPos.y + 10.0f;

	//// Orient to entity
	//glm::quat newOri = glm::quat(
	//	glm::inverse(
	//		glm::lookAt(this->eye, this->eye - direction, glm::vec3(0.0f, 1.0f, 0.0f))
	//	));
	//this->setMeshOrientationQ(newOri);

	//return;
}