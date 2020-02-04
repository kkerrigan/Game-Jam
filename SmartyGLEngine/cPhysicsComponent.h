#pragma once
#include "cComponent.h"
#include "../physics_interfaces/physics/interfaces/iRigidBody.h"
#include "../physics_interfaces/physics/defs/sRigidBodyDef.h"

/*!	\class	cPhysicsComponent
			\brief  
			\author Daniel Maclam
			\date	2019-02-01 3:17 PM
			*/
		class cPhysicsComponent : public cComponent
		{
			physics::interfaces::iRigidBody* rigid_body;

		public:
			cPhysicsComponent() : cComponent(10), rigid_body(nullptr) {	}

			~cPhysicsComponent()
			{
				//delete this->rigid_body;
				//this->rigid_body = nullptr;
			}

			physics::interfaces::iRigidBody* get_rigid_body();
			void configure_component(physics::interfaces::iRigidBody* r_body);
		};
