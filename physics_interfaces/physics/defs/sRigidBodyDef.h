#pragma once
#include <glm/vec3.hpp>
#include "../interfaces/iShape.h"

namespace physics
{
	namespace defs
	{
		/*!	\class	sRigidBodyDef
			\brief  
			\author Daniel Maclam
			\date	2019-02-01 2:48 PM
			*/
		struct sRigidBodyDef
		{
			float mass;
			glm::vec3 position;
			glm::vec3 scale;
			glm::vec3 orientation;
			glm::vec3 acceleration;
			glm::vec3 velocity;
	
			float radius; //sphere
		
			glm::vec3 p1; //plane
			glm::vec3 p2; //plane
			glm::vec3 p3; //plane

			glm::vec3 half_lengths; //box
			float height; //capsule

			interfaces::iShape::shape_types shape_type;

			sRigidBodyDef();
			sRigidBodyDef(
				float mass, 
				glm::vec3 pos,
				glm::vec3 scale,
				glm::vec3 orientation,
				glm::vec3 accel,
				glm::vec3 vel
			)
			{
				this->mass = mass;
				this->position = pos;
				this->scale = scale;
				this->orientation = orientation;
				this->acceleration = accel;
				this->velocity = vel;
			}
		};

		inline sRigidBodyDef::sRigidBodyDef(): mass(0), position(glm::vec3(0.0f)), scale(glm::vec3(1.0f)), orientation(glm::vec3(0.0f)), acceleration(glm::vec3(0.0f)), velocity(glm::vec3(0.0f)),
		                                       radius(0), p1(glm::vec3(0.0f)),
		                                       p2(glm::vec3(0.0f)), p3(glm::vec3(0.0f)),
		                                       shape_type()
		{
		}
	}
}
