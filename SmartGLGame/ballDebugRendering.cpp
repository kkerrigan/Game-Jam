#include <iDebugRenderer.h>
#include <cDebugRenderComponent.h>
#include <cEulerMovementComponent.h>
#include <cTransformComponent.h>
#include <cEntity.h>

void ballDebugRendering(cEntity* entity) {

	cTransformComponent* transformComponent = static_cast<cTransformComponent*>(entity->components[1]);
	cEulerMovementComponent* eulerMovmentComponent = static_cast<cEulerMovementComponent*>(entity->components[2]);
	cDebugRenderComponent* debugRenderComponent = static_cast<cDebugRenderComponent*>(entity->components[4]);

	// object velocity
	//debugRenderComponent->debugRendererImpl->addLine(transformComponent->position, (transformComponent->position + eulerMovmentComponent->velocity), glm::vec3(1.0f,0.0f,0.0f));

	// object acceleration
	//debugRenderComponent->debugRendererImpl->addLine(transformComponent->position, (transformComponent->position + eulerMovmentComponent->acceleration), glm::vec3(0.0f, 0.0f, 1.0f));
}