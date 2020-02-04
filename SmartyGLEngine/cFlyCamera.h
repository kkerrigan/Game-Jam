#ifndef _cFlyCamera_HG_
#define _cFlyCamera_HG_

#include <glm/glm.hpp>

#include "cEntity.h"

class cFlyCamera {
public:
	cFlyCamera();
	
	float movementSpeed;
	glm::vec3 eye;
	glm::vec3 m_up;
	glm::vec3 m_at;		
	glm::vec3 getAtInWorldSpace(void);
	glm::vec3 getCameraDirection(void);
	glm::vec3 getUpVector(void);

	// Used to follow the selected entity
	float minFollowDistance; // Will default to 20.0f;
	float cameraMoveSpeed; // Default to 30.0f

	// These set methods will also clear the "last" mouse positions
	void setMouseXY(double newX, double newY);
	void setMouseWheelDelta(double deltaWheel); 
	float getMouseX(void);				
	float getMouseY(void);
	float getDeltaMouseX(void);
	float getDeltaMouseY(void);
	float getMouseWheel(void);

	void MoveForward_Z(float amount);
	void MoveLeftRight_X(float amount);
	void MoveUpDown_Y(float amount);

	void Pitch_UpDown(float angleDegrees);	// around X
	void Yaw_LeftRight(float angleDegrees);	// around y
	void Roll_CW_CCW(float angleDegrees);	// around z

	glm::quat getQOrientation(void) { return this->qOrientation; };
	void setMeshOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees = false);
	void setMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees = false);
	void adjMeshOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationQ(glm::quat adjOrientQ);
	void setMeshOrientationQ(glm::quat adjOrientQ);

	// Follow objects
	void followSelectedPhysicsEntity(float deltaTime, cEntity* pEntity);

private:
	// If we are using the mouse to change direction...
	float m_lastMouse_X;
	float m_lastMouse_Y;
	float m_lastMouse_Wheel;
	float m_Mouse_X;
	float m_Mouse_Y;
	float m_MouseWheel;
	bool m_Mouse_Initial_State_Is_Set;
	
	// THESE DON'T LIKELY CHANGE
	glm::vec3 m_frontOfCamera;
	glm::vec3 m_upIsYVector;

	// This will lead to direction, etc.
	glm::quat qOrientation;

	void m_UpdateAtFromOrientation(void);
	void m_UpdateUpFromOrientation(void);
};

#endif