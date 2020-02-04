/*
* All the commands for the mediator (This will be cleaned up and moved into thier own files
* to limit the impact on build time (this is here to demo easily)
*/

#ifndef _iCommand_HG_
#define _iCommand_HG_

#include <iostream>

#include "cFlyCamera.h"

class iInputCommand {
public:
	virtual void execute(void* thingIAmDoingWorkOn) = 0;
	virtual ~iInputCommand() {}

};

class cMoveCameraForwardCommand : public iInputCommand{
	float distance;
public:
	cMoveCameraForwardCommand(float _distance) : distance(_distance) {}
	void execute(void* thingIAmDoingWorkOn) {
		((cFlyCamera*)thingIAmDoingWorkOn)->MoveForward_Z(distance);
	}
};

class cMoveCameraLeftRightCommand : public iInputCommand {
	float distance;
public:
	cMoveCameraLeftRightCommand(float _distance) : distance(_distance) {}
	void execute(void* thingIAmDoingWorkOn) {
		((cFlyCamera*)thingIAmDoingWorkOn)->MoveLeftRight_X(distance);
	}
};

class cMoveCameraUpDownCommand : public iInputCommand {
	float distance;
public:
	cMoveCameraUpDownCommand(float _distance) : distance(_distance) {}
	void execute(void* thingIAmDoingWorkOn) {
		((cFlyCamera*)thingIAmDoingWorkOn)->MoveUpDown_Y(distance);
	}
};

class cMoveLightForwardCommand : public iInputCommand {
	float distance;
public:
	cMoveLightForwardCommand(float _distance) : distance(_distance) {}
	void execute(void* thingIAmDoingWorkOn) {
		((sLight*)thingIAmDoingWorkOn)->position.z += distance;
	}
};

class cMoveLightLeftRightCommand : public iInputCommand {
	float distance;
public:
	cMoveLightLeftRightCommand(float _distance) : distance(_distance) {}
	void execute(void* thingIAmDoingWorkOn) {
		((sLight*)thingIAmDoingWorkOn)->position.x += distance;
	}
};

class cMoveLightUpDownCommand : public iInputCommand {
	float distance;
public:
	cMoveLightUpDownCommand(float _distance) : distance(_distance) {}
	void execute(void* thingIAmDoingWorkOn) {
		((sLight*)thingIAmDoingWorkOn)->position.y += distance;
	}
};

class cLightChangeConstAttenuationCommand : public iInputCommand {
	float multiplier;
public:
	cLightChangeConstAttenuationCommand(float _multiplier) : multiplier(_multiplier) {}
	void execute(void* thingIAmDoingWorkOn) {
		((sLight*)thingIAmDoingWorkOn)->atten.x *= multiplier;
	}
};

class cLightChangeLinAttenuationCommand : public iInputCommand {
	float multiplier;
public:
	cLightChangeLinAttenuationCommand(float _multiplier) : multiplier(_multiplier) {}
	void execute(void* thingIAmDoingWorkOn) {
		((sLight*)thingIAmDoingWorkOn)->atten.y *= multiplier;
	}
};

class cLightChangeQuadAttenuationCommand : public iInputCommand {
	float multiplier;
public:
	cLightChangeQuadAttenuationCommand(float _multiplier) : multiplier(_multiplier) {}
	void execute(void* thingIAmDoingWorkOn) {
		((sLight*)thingIAmDoingWorkOn)->atten.z *= multiplier;
	}
};

class cMoveEntiyForwardCommand : public iInputCommand {
	cEntity* selectedEntity;
public:
	cMoveEntiyForwardCommand(cEntity* entity) : selectedEntity(entity) {}
	void execute(void* thingIAmDoingWorkOn) {

		glm::vec4 vecForwardDirection_ModelSpace = glm::vec4(0.0f, 0.0f, /**/1.0f/**/, 1.0f);
		float deltaTime = float(*((double*)thingIAmDoingWorkOn));
		
		// Now orientation
		glm::quat objectOrientation = ((cTransformComponent*)selectedEntity->components[1])->getQOrientation();

		// Make a mat4x4 from that quaternion
		glm::mat4 matQObjrotation = glm::mat4(objectOrientation);
		glm::vec4 vecForwardDirection_WorldSpace = matQObjrotation * vecForwardDirection_ModelSpace;
		
		// optional normalize
		vecForwardDirection_WorldSpace = glm::normalize(vecForwardDirection_WorldSpace);
		

		((cEulerMovementComponent*)selectedEntity->components[2])->acceleration += glm::vec3(vecForwardDirection_WorldSpace) * 15.0f * deltaTime;
	}
};

class cMoveEntiyBackwardCommand : public iInputCommand {
	cEntity* selectedEntity;
public:
	cMoveEntiyBackwardCommand(cEntity* entity) : selectedEntity(entity) {}
	void execute(void* thingIAmDoingWorkOn) {

		glm::vec4 vecBackwardDirection_ModelSpace = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
		float deltaTime = float(*((double*)thingIAmDoingWorkOn));

		// Now orientation
		glm::quat objectOrientation = ((cTransformComponent*)selectedEntity->components[1])->getQOrientation();

		// Make a mat4x4 from that quaternion
		glm::mat4 matQObjrotation = glm::mat4(objectOrientation);
		glm::vec4 vecBackwardDirection_WorldSpace = matQObjrotation * vecBackwardDirection_ModelSpace;

		// optional normalize
		vecBackwardDirection_WorldSpace = glm::normalize(vecBackwardDirection_WorldSpace);


		((cEulerMovementComponent*)selectedEntity->components[2])->acceleration += glm::vec3(vecBackwardDirection_WorldSpace) * 15.0f * deltaTime;
	}
};

class cMoveEntiyLeftCommand : public iInputCommand {
	cEntity* selectedEntity;
public:
	cMoveEntiyLeftCommand(cEntity* entity) : selectedEntity(entity) {}
	void execute(void* thingIAmDoingWorkOn) {
		
		glm::vec4 vecLeftDirection_ModelSpace = glm::vec4(1.0f, 0.0f,0.0f, 1.0f);
		float deltaTime = float(*((double*)thingIAmDoingWorkOn));

		// Now orientation
		glm::quat objectOrientation = ((cTransformComponent*)selectedEntity->components[1])->getQOrientation();

		// Make a mat4x4 from that quaternion
		glm::mat4 matQObjrotation = glm::mat4(objectOrientation);
		glm::vec4 vecLeftDirection_WorldSpace = matQObjrotation * vecLeftDirection_ModelSpace;

		// optional normalize
		vecLeftDirection_WorldSpace = glm::normalize(vecLeftDirection_WorldSpace);


		((cEulerMovementComponent*)selectedEntity->components[2])->acceleration += glm::vec3(vecLeftDirection_WorldSpace) * 15.0f * deltaTime;
	}
};

class cMoveEntiyRightCommand : public iInputCommand {
	cEntity* selectedEntity;
public:
	cMoveEntiyRightCommand(cEntity* entity) : selectedEntity(entity) {}
	void execute(void* thingIAmDoingWorkOn) {

		glm::vec4 vecRightDirection_ModelSpace = glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f);
		float deltaTime = float(*((double*)thingIAmDoingWorkOn));

		// Now orientation
		glm::quat objectOrientation = ((cTransformComponent*)selectedEntity->components[1])->getQOrientation();

		// Make a mat4x4 from that quaternion
		glm::mat4 matQObjrotation = glm::mat4(objectOrientation);
		glm::vec4 vecRightDirection_WorldSpace = matQObjrotation * vecRightDirection_ModelSpace;

		// optional normalize
		vecRightDirection_WorldSpace = glm::normalize(vecRightDirection_WorldSpace);


		((cEulerMovementComponent*)selectedEntity->components[2])->acceleration += glm::vec3(vecRightDirection_WorldSpace) * 15.0f * deltaTime;
	}
};

class cMoveEntiyUpCommand : public iInputCommand {
	cEntity* selectedEntity;
public:
	cMoveEntiyUpCommand(cEntity* entity) : selectedEntity(entity) {}
	void execute(void* thingIAmDoingWorkOn) {
		
		glm::vec4 vecUpDirection_ModelSpace = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		float deltaTime = float(*((double*)thingIAmDoingWorkOn));

		// Now orientation
		glm::quat objectOrientation = ((cTransformComponent*)selectedEntity->components[1])->getQOrientation();

		// Make a mat4x4 from that quaternion
		glm::mat4 matQObjrotation = glm::mat4(objectOrientation);
		glm::vec4 vecUpDirection_WorldSpace = matQObjrotation * vecUpDirection_ModelSpace;

		// optional normalize
		vecUpDirection_WorldSpace = glm::normalize(vecUpDirection_WorldSpace);


		((cEulerMovementComponent*)selectedEntity->components[2])->acceleration += glm::vec3(vecUpDirection_WorldSpace) * 15.0f * deltaTime;
	}
};

class cMoveEntiyDownCommand : public iInputCommand {
	cEntity* selectedEntity;
public:
	cMoveEntiyDownCommand(cEntity* entity) : selectedEntity(entity) {}
	void execute(void* thingIAmDoingWorkOn) {

		glm::vec4 vecDownDirection_ModelSpace = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
		float deltaTime = float(*((double*)thingIAmDoingWorkOn));

		// Now orientation
		glm::quat objectOrientation = ((cTransformComponent*)selectedEntity->components[1])->getQOrientation();

		// Make a mat4x4 from that quaternion
		glm::mat4 matQObjrotation = glm::mat4(objectOrientation);
		glm::vec4 vecDownDirection_WorldSpace = matQObjrotation * vecDownDirection_ModelSpace;

		// optional normalize
		vecDownDirection_WorldSpace = glm::normalize(vecDownDirection_WorldSpace);


		((cEulerMovementComponent*)selectedEntity->components[2])->acceleration += glm::vec3(vecDownDirection_WorldSpace) * 15.0f * deltaTime;
	}
};

class cRotateEntiyZCommand : public iInputCommand {
	float delta;
public:
	cRotateEntiyZCommand(float delta_) : delta(delta_) {}
	void execute(void* thingIAmDoingWorkOn) {
		((cTransformComponent*)thingIAmDoingWorkOn)->adjOrientationEulerAngles(glm::vec3(0.0f, 0.0f, delta), true);
	}
};

class cRotateEntiyXCommand : public iInputCommand {
	float delta;
public:
	cRotateEntiyXCommand(float delta_) : delta(delta_) {}
	void execute(void* thingIAmDoingWorkOn) {
		((cTransformComponent*)thingIAmDoingWorkOn)->adjOrientationEulerAngles(glm::vec3(delta, 0.0f, 0.0f), true);
	}
};

class cRotateEntiyYCommand : public iInputCommand {
	float delta;
public:
	cRotateEntiyYCommand(float delta_) : delta(delta_) {}
	void execute(void* thingIAmDoingWorkOn) {
		((cTransformComponent*)thingIAmDoingWorkOn)->adjOrientationEulerAngles(glm::vec3(0.0f, delta, 0.0f), true);
	}
};

class cChangeEntitySpecularPower : public iInputCommand {
	float multiplyer;
public:
	cChangeEntitySpecularPower(float multiplyer_) : multiplyer(multiplyer_) {}
	void execute(void* thingIAmDoingWorkOn) {
		((cRenderMeshComponent*)thingIAmDoingWorkOn)->specular.w *= multiplyer;

			if (((cRenderMeshComponent*)thingIAmDoingWorkOn)->specular.w < 1.0) {
				((cRenderMeshComponent*)thingIAmDoingWorkOn)->specular.w = 1.0f;
			}
	}
};

#endif
