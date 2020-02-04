#pragma once
#include <glm/vec3.hpp>
#include "iShape.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace physics
{
	namespace interfaces
	{
		/*!	\class	iRigidBody
			\brief  
			\author Daniel Maclam
			\date	2019-02-01 2:43 PM
			*/
		class iRigidBody
		{
		public:
			virtual ~iRigidBody() = default;

			virtual glm::vec3 get_position() = 0;
			virtual void set_position(glm::vec3 position) = 0;

			virtual glm::vec3 get_previous_position() = 0;
			virtual void set_previous_position(glm::vec3 position) = 0;

			virtual glm::vec3 get_scale() =0;
			virtual void set_scale(glm::vec3 scale) = 0;

			virtual glm::quat get_orientation() = 0;
			virtual void set_orientation(glm::quat quat) = 0;

			virtual glm::vec3 get_acceleration() = 0;
			virtual void set_acceleration(glm::vec3 accel) = 0;

			virtual glm::vec3 get_velocity() = 0;
			virtual void set_velocity(glm::vec3 vel) = 0;

			virtual float get_mass() = 0;
			virtual void set_mass(float mass) = 0;

			virtual iShape* get_shape() = 0;
			virtual iShape::shape_types get_shape_type() = 0;

			virtual void apply_force(glm::vec3 acceleration) = 0;
		};
	}
}
