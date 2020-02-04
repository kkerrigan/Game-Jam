#include "Global.h"

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "cEngine.h"
#include "cDebugRenderSystem.h"
#include "cDebugRenderComponent.h"
#include "cDebugRenderer.h"
#include "cSceneManager.h"

cDebugRenderSystem* cDebugRenderSystem::getInstance() {
	static cDebugRenderSystem instance;

	return &instance;
}

void cDebugRenderSystem::update(double deltaTime) {
	
	cDebugRenderComponent* debugRenderComponent = nullptr;
	for (unsigned int objIndex = 0; objIndex != this->entitiesToUpdate.size(); objIndex++)
	{
		cEntity* entity = this->entitiesToUpdate[objIndex];
		debugRenderComponent = static_cast<cDebugRenderComponent*>(entity->components[4]);
		debugRenderComponent->callback(entity);
	}

	glm::mat4x4 matProjection = glm::mat4(1.0f);
	glm::mat4x4	matView = glm::mat4(1.0f);

	float ratio;
	int width, height;

	glfwGetFramebufferSize(cEngine::getInstance()->getWindow(), &width, &height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);

	matProjection = glm::perspective(0.6f,			// FOV
		ratio,		// Aspect ratio
		0.1f,			// Near clipping plane
		1000.0f);	// Far clipping plane

	matView = glm::lookAt(cSceneManager::getInstance()->getActiveScene()->cameraPos, cSceneManager::getInstance()->getActiveScene()->cameraLookAt, cSceneManager::getInstance()->getActiveScene()->cameraUp);

	
}

void cDebugRenderSystem::registerEntity(cEntity* entity) {
	this->entitiesToUpdate.push_back(entity);
}

void cDebugRenderSystem::unregisterEntity(cEntity* entity) {
	// TODO: remove from vector
}