#include "cEulerMovementSystem.h"



cEulerMovementSystem::cEulerMovementSystem(){}

cEulerMovementSystem::~cEulerMovementSystem(){}


cEulerMovementSystem* cEulerMovementSystem::getInstance() {
	static cEulerMovementSystem instance;
	
	return &instance;
}

void cEulerMovementSystem::update(double deltaTime) {
	
	for (cEntity* entity : this->entitiesToUpdate) {

		//make sure the entity has all the components we need. if not unregister automagically
		if ((entity->componentBitField & 3) != 3) {
			this->unregisterEntity(entity);
			continue;
		}

		//get the entities position
		cTransformComponent* position = static_cast<cTransformComponent*>(entity->components[1]); // static cast good as we know the component is there

		//get the entities euler velocity and acceleration
		cEulerMovementComponent* eulerMovement = static_cast<cEulerMovementComponent*>(entity->components[2]); // static cast good as we know the component is there

		// acceleration 
		eulerMovement->velocity += eulerMovement->acceleration * (float)deltaTime;
		
		if (eulerMovement->velocity.x > 75.0f) {
			eulerMovement->velocity.x = 75.0f;
		}

		if (eulerMovement->velocity.x < -75.0f) {
			eulerMovement->velocity.x = -75.0f;
		}

		if (eulerMovement->velocity.y > 75.0f) {
			eulerMovement->velocity.y = 75.0f;
		}		

		if (eulerMovement->velocity.y < -75.0f) {
			eulerMovement->velocity.y = -75.0f;
		}

		if (eulerMovement->velocity.z > 75.0f) {
			eulerMovement->velocity.z = 75.0f;
		}

		if (eulerMovement->velocity.z < -75.0f) {
			eulerMovement->velocity.z = -75.0f;
		}

		//movement
		position->set_position(position->get_position() + eulerMovement->velocity * (float)deltaTime);
		//eulerMovement->acceleration *= 0.95f;

		position->lastPosition = position->get_position();
	}
}

void cEulerMovementSystem::registerEntity(cEntity* entity) {
	this->entitiesToUpdate.push_back(entity);
}

void cEulerMovementSystem::unregisterEntity(cEntity* entity) {
	// TODO: remove from vector
}