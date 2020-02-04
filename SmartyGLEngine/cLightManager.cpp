#include "cLightManager.h"
#include "cShaderManager.h"

cLightManager::cLightManager(){
}

cLightManager::~cLightManager(){
}

cLightManager* cLightManager::getInstance() {
	static cLightManager instance;

	return &instance;
}

void cLightManager::initializeLights() {

	int program = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");

	for (int i = 0; i < MAX_NUM_OF_LIGHTS; ++i) {
		sLight* pTheOneLight = new sLight();
		std::string uniformIndex = std::string("theLights[" + std::to_string(i) + "]");
		pTheOneLight->position_UniLoc = glGetUniformLocation(program, (uniformIndex + ".position").c_str());
		pTheOneLight->diffuse_UniLoc = glGetUniformLocation(program, (uniformIndex + ".diffuse").c_str());
		pTheOneLight->specular_UniLoc = glGetUniformLocation(program, (uniformIndex + ".specular").c_str());
		pTheOneLight->atten_UniLoc = glGetUniformLocation(program, (uniformIndex + ".atten").c_str());
		pTheOneLight->direction_UniLoc = glGetUniformLocation(program, (uniformIndex + ".direction").c_str());
		pTheOneLight->param1_UniLoc = glGetUniformLocation(program, (uniformIndex + ".param1").c_str());
		pTheOneLight->param2_UniLoc = glGetUniformLocation(program, (uniformIndex + ".param2").c_str());

		pTheOneLight->position = glm::vec4(0.0f, 500.0f, 0.0f, 1.0f);
		pTheOneLight->atten.x = 0.0f;	// 			float constAtten = 0.0f;
		pTheOneLight->atten.y = 0.001f;	//			float linearAtten = 0.01f;
		pTheOneLight->atten.z = 0.0000001f;	//			float quadAtten = 0.001f;
		pTheOneLight->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);		// White light

		this->allocatedLights.push_back(pTheOneLight);
	}
}

void cLightManager::writeLightsToShader() {
	int program = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
	cShaderManager::getInstance()->useShaderProgram(program);

	for (int i = 0; i < MAX_NUM_OF_LIGHTS; ++i) {
		glUniform4f(this->allocatedLights[i]->position_UniLoc, this->allocatedLights[i]->position.x, this->allocatedLights[i]->position.y, this->allocatedLights[i]->position.z, 1.0f);
		glUniform4f(this->allocatedLights[i]->diffuse_UniLoc, this->allocatedLights[i]->diffuse.x, this->allocatedLights[i]->diffuse.y, this->allocatedLights[i]->diffuse.z, 1.0f);
		glUniform4f(this->allocatedLights[i]->param2_UniLoc, this->allocatedLights[i]->param2.x, 0.0f, 0.0f, 0.0f);	// Turns it "on")
		glUniform4f(this->allocatedLights[i]->atten_UniLoc, this->allocatedLights[i]->atten.x, this->allocatedLights[i]->atten.y, this->allocatedLights[i]->atten.z, this->allocatedLights[i]->atten.w);
		

		glUniform4f(
			this->allocatedLights[i]->direction_UniLoc, this->allocatedLights[i]->direction.x, this->allocatedLights[i]->direction.y, this->allocatedLights[i]->direction.z, this->allocatedLights[i]->direction.w);
		glUniform4f(
			this->allocatedLights[i]->param1_UniLoc, this->allocatedLights[i]->param1.x, this->allocatedLights[i]->param1.y, this->allocatedLights[i]->param1.z, this->allocatedLights[i]->param1.w);	
	}
}