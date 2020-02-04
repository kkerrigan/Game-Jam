/*
* Name:		cRigidBody.h
* Author:	Daniel Maclam
* Date		2019-02-11 8:32 PM
*/

#include "cRigidBody.h"
#include "bullet_adapters/bullet_adapters.h"

physics::cRigidBody::cRigidBody(btRigidBody* bullet_body, glm::vec3 scale) : rigid_body(bullet_body), scale(scale) {
	this->rigid_body = bullet_body;
}

btRigidBody* physics::cRigidBody::get_body() const
{
	return this->rigid_body;
}

glm::vec3 physics::cRigidBody::get_position()
{
	return bullet_adapters::to_glm(this->rigid_body->getCenterOfMassPosition());
}

void physics::cRigidBody::set_position(glm::vec3 position)
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(bullet_adapters::to_bullet(position));

	this->rigid_body->setWorldTransform(transform);
	this->rigid_body->activate();
}

glm::vec3 physics::cRigidBody::get_previous_position()
{
	return glm::vec3(0.0f);
}

//TODO: real implementation
void physics::cRigidBody::set_previous_position(glm::vec3 position)
{
	
}

glm::vec3 physics::cRigidBody::get_scale()
{
	return this->scale;
}

void physics::cRigidBody::set_scale(const glm::vec3 scale)
{
	this->scale = scale;
}

glm::quat physics::cRigidBody::get_orientation()
{
	return bullet_adapters::to_glm(this->rigid_body->getOrientation());
}

void physics::cRigidBody::set_orientation(glm::quat quaternion)
{
	const btQuaternion orientation = bullet_adapters::to_bullet(quaternion);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(this->rigid_body->getCenterOfMassPosition());
	transform.setRotation(orientation);

	this->rigid_body->setWorldTransform(transform);
	this->rigid_body->activate();
}


glm::vec3 physics::cRigidBody::get_acceleration()
{
	return bullet_adapters::to_glm(this->rigid_body->getLinearFactor());
}

void physics::cRigidBody::set_acceleration(glm::vec3 accel)
{
	this->rigid_body->applyCentralForce(bullet_adapters::to_bullet(accel));
}

glm::vec3 physics::cRigidBody::get_velocity()
{
	
	return bullet_adapters::to_glm(this->rigid_body->getInterpolationLinearVelocity());
	
}

void physics::cRigidBody::set_velocity(glm::vec3 vel)
{
	this->rigid_body->setLinearVelocity(bullet_adapters::to_bullet(vel));
}

float physics::cRigidBody::get_mass()
{
	return 0.0f;
}

void physics::cRigidBody::set_mass(float mass)
{

}

physics::interfaces::iShape* physics::cRigidBody::get_shape()
{
	return this->shape;
}

physics::interfaces::iShape::shape_types physics::cRigidBody::get_shape_type()
{
	return this->shape->get_shape_type();
}

void physics::cRigidBody::apply_force(glm::vec3 acceleration)
{
	this->rigid_body->activate();
	this->rigid_body->applyCentralForce(bullet_adapters::to_bullet(acceleration));
}