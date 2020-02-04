/*
* Name:		cRigidBody.h
* Author:	Daniel Maclam
* Date		2019-02-01 2:45 PM
*/

#include "cRigidBody.h"

physics::cRigidBody::cRigidBody(glm::vec3 position, glm::vec3 scale, glm::vec3 orientation, glm::vec3 acceleration,
                                glm::vec3 velocity, interfaces::iShape* shape, float mass): shape(shape),
                                                                                            position(position),
                                                                                            previous_position(position),
                                                                                            scale(scale),
                                                                                            acceleration(acceleration),
                                                                                            velocity(velocity),
                                                                                            mass(mass)
{


	orientation = glm::vec3(glm::radians(orientation.x), glm::radians(orientation.y), glm::radians(orientation.z));
	this->orientation = glm::quat(glm::vec3(orientation.x, orientation.y, orientation.z));
}

glm::vec3 physics::cRigidBody::get_position()
{
	return this->position;
}

void physics::cRigidBody::set_position(glm::vec3 position)
{
	this->position = position;
}

glm::vec3 physics::cRigidBody::get_scale()
{
	return this->scale;
}

void physics::cRigidBody::set_scale(glm::vec3 scale)
{
	this->scale = scale;
}

glm::quat physics::cRigidBody::get_orientation()
{
	return this->orientation;
}

void physics::cRigidBody::set_orientation(glm::quat quat)
{
	this->orientation = quat;
}

glm::vec3 physics::cRigidBody::get_previous_position()
{
	return this->previous_position;
}

void physics::cRigidBody::set_previous_position(glm::vec3 position)
{
	this->previous_position = position;
}

glm::vec3 physics::cRigidBody::get_acceleration()
{
	return this->acceleration;
}

void physics::cRigidBody::set_acceleration(glm::vec3 accel)
{
	this->acceleration = accel;
}

glm::vec3 physics::cRigidBody::get_velocity()
{
	return this->velocity;
}

void physics::cRigidBody::set_velocity(glm::vec3 vel)
{
	this->velocity = vel;
}

float physics::cRigidBody::get_mass()
{
	return this->mass;
}

void physics::cRigidBody::set_mass(float mass)
{
	this->mass = mass;
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
	this->acceleration += acceleration;
}
