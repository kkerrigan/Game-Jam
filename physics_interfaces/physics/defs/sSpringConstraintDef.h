#pragma once
#include <glm/vec3.hpp>
#include "../interfaces/iRigidBody.h"
#include "../constraints/iConstraint.h"
namespace physics
{
	namespace defs
	{
		/*!	\class	sSpringConstraintDef
		\brief
		\author Daniel Maclam
		\date	2019-03-23 1:27 PM
		*/
		class sSpringConstraintDef
		{
		public:
			constraints::constraint_types type;
			physics::interfaces::iRigidBody* rbA;
			physics::interfaces::iRigidBody* rbB;

			float stiffness;
			bool use_linear_reference_frame;

			glm::vec3 frame_in_a;
			glm::vec3 frame_in_b;
		};
	}

}
