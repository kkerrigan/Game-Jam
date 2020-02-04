#pragma once

#include <physics/interfaces/iRigidBody.h>
#include <btBulletDynamicsCommon.h>

namespace physics
{
	/*!	\class	cRigidBody
		\brief
		\author Daniel Maclam
		\date	2019-02-01 2:45 PM
		*/
	class cRigidBody : public interfaces::iRigidBody
	{
		interfaces::iShape* shape;
		btRigidBody* rigid_body;

		glm::vec3 scale;

	public:
		cRigidBody(btRigidBody* bullet_body, glm::vec3 scale);

		glm::vec3 get_position() override;
		void set_position(glm::vec3 position) override;

		glm::vec3 get_scale() override;
		void set_scale(glm::vec3 scale) override;

		glm::quat get_orientation() override;
		void set_orientation(glm::quat quaternion) override;

		glm::vec3 get_previous_position() override;
		void set_previous_position(glm::vec3 position) override;

		glm::vec3 get_acceleration() override;
		void set_acceleration(glm::vec3 accel) override;

		glm::vec3 get_velocity() override;
		void set_velocity(glm::vec3 vel) override;

		float get_mass() override;
		void set_mass(float mass) override;

		interfaces::iShape* get_shape() override;
		interfaces::iShape::shape_types get_shape_type() override;

		void apply_force(glm::vec3 acceleration) override;

		btRigidBody* get_body() const;
	};
}

