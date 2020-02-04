/*
* Name:		cPhysicsWorld.h
* Author:	Daniel Maclam
* Date		2019-02-11 7:07 PM
*/

#include "cPhysicsWorld.h"
#include "bullet_adapters/bullet_adapters.h"
#include "cRigidBody.h"
#include "physics/interfaces/shapes/iSphere.h"
#include "shapes/cHeightmapTerrain.h"

physics::cPhysicsWorld::cPhysicsWorld()
{
	this->collisionConfiguration = new btDefaultCollisionConfiguration();

	//use the default collision dispatcher.
	this->dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//btDbvtBroadphase is a good general purpose broad phase. You can also try out btAxis3Sweep.
	this->overlappingPairCache = new btDbvtBroadphase();

	//the default constraint solver.
	this->solver = new btSequentialImpulseConstraintSolver;

	this->dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
	this->dynamicsWorld->setGravity(btVector3(0, -50.0, 0));
}

physics::cPhysicsWorld::~cPhysicsWorld()
{
	//cleanup the physics objects
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;

		for (int j = 0; j < collisionShapes.size(); j++)
		{
			btCollisionShape* shape = collisionShapes[j];
			collisionShapes[j] = nullptr;
			delete shape;
		}

		//delete dynamics world
		delete dynamicsWorld;

		//delete solver
		delete solver;

		//delete broad phase
		delete overlappingPairCache;

		//delete dispatcher
		delete dispatcher;

		//delete configuration
		delete collisionConfiguration;

		//clear the vector of shapes
		collisionShapes.clear();
	}
}

void physics::cPhysicsWorld::register_heightmap_terrain(interfaces::iHeightMapTerrain* terrain)
{
	btRigidBody* test = static_cast<btRigidBody*>(terrain->get_shape());

	this->dynamicsWorld->addRigidBody(test);

}

void physics::cPhysicsWorld::update(const float delta_time)
{
	this->dynamicsWorld->stepSimulation(delta_time, 0, 1.f/60.f );
}

void physics::cPhysicsWorld::register_rigid_body(physics::interfaces::iRigidBody* body)
{
	this->dynamicsWorld->addRigidBody(static_cast<cRigidBody*>(body)->get_body());
}

void physics::cPhysicsWorld::unregister_rigid_body(physics::interfaces::iRigidBody* body)
{
	this->dynamicsWorld->removeRigidBody(static_cast<cRigidBody*>(body)->get_body());
}