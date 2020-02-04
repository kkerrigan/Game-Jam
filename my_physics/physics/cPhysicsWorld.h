#pragma once
#include <physics/interfaces/iPhysicsWorld.h>
#include <vector>
#include "physics/interfaces/shapes/iSphere.h"
#include <physics/interfaces/iRigidBody.h>
#include "physics/interfaces/shapes/iPlane.h"

namespace physics
{
	/*!	\class	cPhysicsWorld
		\brief
		\author Daniel Maclam
		\date	2019-02-01 2:01 PM
		*/
	class cPhysicsWorld : public interfaces::iPhysicsWorld
	{
		struct State
		{
			State(): p(glm::vec3(0.0f)), v(glm::vec3(0.0f)) {}
			glm::vec3 p;      // position
			glm::vec3 v;      // velocity
		};

		struct Derivative
		{
			Derivative():dx(glm::vec3(0.0f)), dv(glm::vec3(0.0f)) {}

			glm::vec3 dx;      // dx/dt = velocity
			glm::vec3 dv;      // dv/dt = acceleration
		};

	private:
		float total_time = 0.0f;

		std::vector<interfaces::iRigidBody*> rigid_bodies;

		int intersect_moving_sphere_plane(interfaces::iRigidBody* s, glm::vec3 movement_vector, interfaces::iRigidBody* p, float &t, glm::vec3 &q) const;
		int intersect_moving_sphere_sphere(interfaces::iRigidBody* s1, interfaces::iRigidBody* s2, float &t) const;

		Derivative evaluate(const State & initial, glm::vec3 t, float dt, const Derivative & d);
		void integrate(State & state, glm::vec3 t,	float dt);

	public:
		void register_rigid_body(interfaces::iRigidBody* body) override;
		void unregister_rigid_body(interfaces::iRigidBody* body) override;

		void update(float delta_time) override;
	};
}
