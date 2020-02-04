#ifndef _sLight_HG_
#define _sLight_HG_

#include "Global.h"

#include <rapidjson/prettywriter.h>

struct sLight {
	enum eLightType
	{					// These numbers are from the shader...
		DIRECTIONAL_LIGHT ,	// = 2
		POINT_LIGHT ,		// = 0
		SPOT_LIGHT 			// = 1
	};

	glm::vec4 position;
	glm::vec4 diffuse;
	glm::vec4 specular;	// rgb = highlight colour, w = power
	glm::vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	glm::vec4 direction;	// Spot, directional lights
	glm::vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	glm::vec4 param2;	// x = 0 for off, 1 for on

	// Would set atten.y = newLinAtten
	void SetLinearAttenuation(float newLinAtten);
	void SetPosition(glm::vec3 newPosition);
	void SetSpecularHighlight(glm::vec3 rgbColour, float power);
	// .. and so on

	void SetLightType(eLightType lightType) {
		switch (lightType)
		{
		case sLight::POINT_LIGHT:
			this->param1.x = 0.0f;		// Point
			break;
		case sLight::SPOT_LIGHT:
			this->param1.x = 1.0f;		// Spot
			break;
		case sLight::DIRECTIONAL_LIGHT:
			this->param1.x = 2.0f;		// Directional
			break;
		default:
			// Make point if we don't know
			// (shouldn't happen)
			this->param1.x = 0.0f;		// Point
			break;
		};

		return;
	}

	void SetSpotConeAngles(float innerAngleDegrees, float outerAngleDegrees) {
		//	glm::vec4 param1;		// x = lightType, y = inner angle, z = outer angle, w = TBD
		this->param1.y = innerAngleDegrees;
		this->param1.z = outerAngleDegrees;
	}

	void SetRelativeDirection(glm::vec3 relDirection) {
		this->direction = glm::vec4(relDirection, 1.0f);
		return;
	}

	void SetRelativeDirectionByEulerAngles(glm::vec3 directionAngle);
	void SetRelativeDirectionByLookAt(glm::vec3 pointInWorldXYZ) {
		glm::vec3 lookVector = pointInWorldXYZ - glm::vec3(this->position);

		lookVector = glm::normalize(lookVector);

		this->SetRelativeDirection(lookVector);
	}

	int position_UniLoc;
	int diffuse_UniLoc;
	int specular_UniLoc;
	int atten_UniLoc;
	int direction_UniLoc;
	int param1_UniLoc;
	int param2_UniLoc;

	sLight() : position(glm::vec4(0.0f)), diffuse(glm::vec4(0.0f)), specular(glm::vec4(0.0f)), atten(glm::vec4(0.0f)), direction(glm::vec4(0.0f)), param1(glm::vec4(0.0f)), param2(glm::vec4(0.0f)) {}

	
	void serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {	
		writer.StartObject();
		writer.String("position");
		writer.StartObject();
			writer.String("x");
			writer.Double(this->position.x);
			writer.String("y");
			writer.Double(this->position.y);
			writer.String("z");
			writer.Double(this->position.z);
		writer.EndObject();

		writer.String("diffuse");
		writer.StartObject();
			writer.String("r");
			writer.Double(this->diffuse.r);
			writer.String("g");
			writer.Double(this->diffuse.g);
			writer.String("b");
			writer.Double(this->diffuse.b);
			writer.String("a");
			writer.Double(this->diffuse.a);
		writer.EndObject();

		writer.String("specular");
		writer.StartObject();
			writer.String("r");
			writer.Double(this->specular.r);
			writer.String("g");
			writer.Double(this->specular.g);
			writer.String("b");
			writer.Double(this->specular.b);
			writer.String("a");
			writer.Double(this->specular.a);
		writer.EndObject();

		writer.String("attenuation");
		writer.StartObject();
			writer.String("constant");
			writer.Double(this->atten.r);
			writer.String("linear");
			writer.Double(this->atten.g);
			writer.String("quadratic");
			writer.Double(this->atten.b);
			writer.String("cutoffDist");
			writer.Double(this->atten.a);
		writer.EndObject();

		writer.String("direction");
		writer.StartObject();
			writer.String("x");
			writer.Double(this->direction.r);
			writer.String("y");
			writer.Double(this->direction.g);
			writer.String("z");
			writer.Double(this->direction.b);
			writer.String("w");
			writer.Double(this->direction.a);
		writer.EndObject();

		writer.String("param1");
		writer.StartObject();
			writer.String("lightType");
			writer.Double(this->param1.r);
			writer.String("innerAngle");
			writer.Double(this->param1.g);
			writer.String("outerAngle");
			writer.Double(this->param1.b);
			writer.String("w");
			writer.Double(this->param1.a);
		writer.EndObject();

		writer.String("param2");
		writer.StartObject();
			writer.String("isOn");
			writer.Double(this->param2.r);
			writer.String("y");
			writer.Double(this->param2.g);
			writer.String("z");
			writer.Double(this->param2.b);
			writer.String("w");
			writer.Double(this->param2.a);
		writer.EndObject();
		writer.EndObject();
	}
};

#endif
