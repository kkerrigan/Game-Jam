#pragma once
#include <glm/vec3.hpp>
#include "../interfaces/iRigidBody.h"
#include "../constraints/iConstraint.h"

namespace physics
{
	namespace defs
	{
		/*!	\class	sHingeConstraintDef
		\brief
		\author Daniel Maclam
		\date	2019-03-23 4:02 AM
		*/
		class sHingeConstraintDef
		{
		public:
			sHingeConstraintDef(): type(constraints::constraint_types::hinge), rbA(nullptr), rbB(nullptr), pivotInA(glm::vec3(0.0f)), pivotInB(glm::vec3(0.0f)), axisInA(glm::vec3(0.0f)), axisInB(glm::vec3(0.0f))
			{
			}

			constraints::constraint_types type;
			physics::interfaces::iRigidBody* rbA;
			physics::interfaces::iRigidBody* rbB;

			glm::vec3 pivotInA;
			glm::vec3 pivotInB;

			glm::vec3 axisInA;
			glm::vec3 axisInB;
		};
	}

}
