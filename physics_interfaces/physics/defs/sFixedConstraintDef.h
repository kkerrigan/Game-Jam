#pragma once
#include "../constraints/iConstraint.h"
#include "../interfaces/iRigidBody.h"

namespace physics
{
	namespace defs
	{
		/*!	\class	sFixedConstraintDef
		\brief
		\author Daniel Maclam
		\date	2019-03-23 2:35 AM
		*/
		class sFixedConstraintDef
		{
		public:
			sFixedConstraintDef(): type(constraints::constraint_types::fixed), rbA(nullptr), rbB(nullptr), pivotInA(glm::vec3(0.0f)), pivotInB(glm::vec3(0.0f))
			{
			}

			constraints::constraint_types type;
			interfaces::iRigidBody* rbA;
			interfaces::iRigidBody* rbB;

			glm::vec3 pivotInA;
			glm::vec3 pivotInB;
		};

	}
}
