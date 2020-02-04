#pragma once
#include "physics/interfaces/iRigidBody.h"


namespace physics
{
	namespace defs
	{
		/*!	\class	sConeTwistConstraintDef
		\brief
		\author Daniel Maclam
		\date	2019-03-23 12:02 PM
		*/
		class sConeTwistConstraintDef
		{
		public:
			constraints::constraint_types type;
			physics::interfaces::iRigidBody* rbA;
			physics::interfaces::iRigidBody* rbB;

			glm::vec3 frame_in_a;
			glm::vec3 frame_in_b;
		};
	}
}
