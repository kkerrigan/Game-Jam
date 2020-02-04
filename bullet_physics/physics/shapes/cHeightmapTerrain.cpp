/*
* Name:		cHeightmapTerrain.h
* Author:	Daniel Maclam
* Date		2019-04-14 6:38 PM
*/

#include "cHeightmapTerrain.h"
#include "BulletCollision/CollisionShapes/btTriangleMesh.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "../bullet_adapters/bullet_adapters.h"
#include "../bullet/BulletDynamics/Dynamics/btRigidBody.h"
#include "LinearMath/btDefaultMotionState.h"

physics::shapes::cHeightmapTerrain::cHeightmapTerrain(int width, int height,std::vector<dsm::sHeightmapVertex> data, float scale, float min_height, float max_height)
{
	this->data = data;

	btTriangleMesh* mesh_interface = new btTriangleMesh();

	for(int i=0; i<data.size(); i++)
	{
		mesh_interface->addTriangle(
			bullet_adapters::to_bullet(data[i].vertexes[0]),
			bullet_adapters::to_bullet(data[i].vertexes[1]),
			bullet_adapters::to_bullet(data[i].vertexes[2]), 
			false
		);
	}

	this->shape = new btBvhTriangleMeshShape(mesh_interface, true, true);
	this->shape->setLocalScaling(btVector3(1.0f, 1.0f,1.0f));

	btDefaultMotionState* motions_state = new btDefaultMotionState(btTransform(
			bullet_adapters::to_bullet(bullet_adapters::create_quaternion_from_euler_angles(glm::vec3(0.0f))),
			bullet_adapters::to_bullet(glm::vec3(0.0,0.0f, 0.0f))));

	r_body = new btRigidBody(0.0,motions_state, this->shape);
}

btRigidBody* physics::shapes::cHeightmapTerrain::get_shape()
{
	return this->r_body;
}
