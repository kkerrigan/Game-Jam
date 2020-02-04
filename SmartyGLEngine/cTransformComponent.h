#ifndef _cTransformComponent_HG_
#define _cTransformComponent_HG_

#include <Windows.h>
#include <process.h>

#include <glm/vec3.hpp>
#include <glm/gtc/constants.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "cComponent.h"

#include <rapidjson/prettywriter.h>

class cTransformComponent : public cComponent {
	glm::vec3 position;
	glm::quat orientation;

	CRITICAL_SECTION position_critical_section;
	CRITICAL_SECTION orientation_critical_section;
public:
	
	glm::vec3 lastPosition;
	glm::vec3 scale;

	cTransformComponent();

	~cTransformComponent();

	void set_position(glm::vec3 position);
	glm::vec3 get_position();

	void serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	
	glm::quat getQOrientation(void);
	void setQOrientation(glm::quat newOrientation);

	void setOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees = false);
	void setOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees = false);
	void adjOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationQ(glm::quat adjOrientQ);
	
};

#endif