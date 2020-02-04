#include "cColliderSystem.h"

#include <algorithm>

#include "cTransformComponent.h"
#include "cEulerMovementComponent.h"
#include "cRenderMeshComponent.h"
#include "cCollider.h"
#include "cPhysics.h"
#include "cDebugRenderComponent.h"
#include "cVAOMeshManager.h"
#include "cDebugRenderer.h"

#define ZERO_VEL_RANGE 0.8f

cColliderSystem* cColliderSystem::getInstance() {
	static cColliderSystem instance;

	return &instance;
}

void cColliderSystem::update(double deltaTime) {
	
	for (std::vector<cEntity*>::iterator itObj1 = this->entitiesToUpdate.begin(); itObj1 != this->entitiesToUpdate.end(); ++itObj1) {

		testForCollisionWithStaticObject(*itObj1);
	}
}

void cColliderSystem::registerEntity(cEntity* entity) {
	this->entitiesToUpdate.push_back(entity);
}

void cColliderSystem::unregisterEntity(cEntity* entity) {
	// TODO: remove from vector
}

void cColliderSystem::setStaticObjectsInfo(cAABBTerrianBroadPhaseHierarchy* ptr) {
	
	this->staticAABBTriangles = ptr;
}

bool cColliderSystem::testForCollision(cEntity* entity1, cEntity* entity2, double deltaTime) {
	
	// NOTE: Keeping this here to be updated for future use with non-static objects
	
	//cTransformComponent* pObj1Transform = static_cast<cTransformComponent*>(entity1->components[1]);
	//cEulerMovementComponent* pObjec1EulerMovement = static_cast<cEulerMovementComponent*>(entity1->components[2]);
	//cCollider* pObj1Collider = static_cast<cCollider*>(entity1->components[5]);

	//cTransformComponent* pObj2Transform = static_cast<cTransformComponent*>(entity2->components[1]);
	//cEulerMovementComponent* pObjec2EulerMovement = static_cast<cEulerMovementComponent*>(entity2->components[2]);
	//cCollider* pObj2Collider = static_cast<cCollider*>(entity2->components[5]);

	////todo: for each of the physics objects on the entity check for a collision
	////for(auto it = pObj1Collider->physics_objects.begin(); it!= pObj1Collider->physics_objects.end())

	//if (pObj1Collider->pTheShape == NULL) { return false; /*print error?*/ }
	//if (pObj2Collider->pTheShape == NULL) { return false; /*print error?*/ }

	//// Sphere - sphere
	//if ((pObj1Collider->shapeType == pObj1Collider->SPHERE) && (pObj2Collider->shapeType == pObj2Collider->SPHERE)) {
	//	return cPhysics::areSpheresPenetrating(entity1, entity2);
	//}
	//else if ((pObj1Collider->shapeType == pObj1Collider->SPHERE) && (pObj2Collider->shapeType == pObj2Collider->TRIANGLE)) {
	//	return cPhysics::sphereTraingleTest(entity1, entity2);
	//}

	//else if ((pObj1Collider->shapeType == pObj1Collider->SPHERE) && (pObj2Collider->shapeType == pObj2Collider->AABB)) {
	//	
	//	std::vector<sClosestPointData> closestPoints;

	//	sModelDrawInfo cubeInfo;
	//	cubeInfo.meshFileName = "cube_flat_shaded_xyz_n.ply";
	//	cVAOMeshManager::getInstance()->FindDrawInfoByModelName(cubeInfo);

	//	cPhysics::calculateClosestPointsOnMesh(cubeInfo, pObj1Transform->position, closestPoints);

	//	int closestIndex = 0;
	//	float minDistance = glm::abs(glm::distance(pObj1Transform->position, closestPoints[0].thePoint));
	//	for (unsigned int i = 0; i < closestPoints.size(); ++i) {
	//		float dist = std::abs(glm::distance(pObj1Transform->position, closestPoints[i].thePoint));
	//		if ( dist < minDistance) {
	//			minDistance = dist;
	//			closestIndex = i;
	//		}
	//	}

	//	sTriangle tri;
	//	int point1 = cubeInfo.pTriangles[closestPoints[closestIndex].triangleIndex].vertex_index_1;
	//	int point2 = cubeInfo.pTriangles[closestPoints[closestIndex].triangleIndex].vertex_index_2;
	//	int point3 = cubeInfo.pTriangles[closestPoints[closestIndex].triangleIndex].vertex_index_3;
	//	
	//	tri.v[0] = glm::vec3(cubeInfo.pVerticesFromFile[point1].x, cubeInfo.pVerticesFromFile[point1].y, cubeInfo.pVerticesFromFile[point1].z);
	//	tri.v[1] = glm::vec3(cubeInfo.pVerticesFromFile[point2].x, cubeInfo.pVerticesFromFile[point2].y, cubeInfo.pVerticesFromFile[point2].z);
	//	tri.v[2] = glm::vec3(cubeInfo.pVerticesFromFile[point3].x, cubeInfo.pVerticesFromFile[point3].y, cubeInfo.pVerticesFromFile[point3].z);
	//	
	//	sSphere* pSphereA = (sSphere*)(pObj1Collider->pTheShape);
	//	sTriangle* pTri = &tri;

	//	glm::vec3 closestPointToTri = cPhysics::closestPtPointTriangle(pObj1Transform->position, pTri->v[0], pTri->v[1], pTri->v[2]);

	//	//// is this point LESS THAN the radius of the sphere? 
	//	float dist = glm::abs(glm::distance(closestPointToTri, pObj1Transform->position));
	//	if (dist <= pSphereA->radius + 0.1f) {
	//		//get the normal
	//		glm::vec3 norm1 = glm::vec3(cubeInfo.pVerticesToVBO[point1].nx, cubeInfo.pVerticesToVBO[point1].ny, cubeInfo.pVerticesToVBO[point1].nz);
	//		glm::vec3 norm2 = glm::vec3(cubeInfo.pVerticesToVBO[point2].nx, cubeInfo.pVerticesToVBO[point2].ny, cubeInfo.pVerticesToVBO[point2].nz);
	//		glm::vec3 norm3 = glm::vec3(cubeInfo.pVerticesToVBO[point3].nx, cubeInfo.pVerticesToVBO[point3].ny, cubeInfo.pVerticesToVBO[point3].nz);

	//		glm::vec3 normal = (norm1 + norm2 + norm3) / glm::vec3(3.0f,3.0f,3.0f);
	//	
	//		glm::vec3 reflected =-normal * (-normal * pObjec1EulerMovement->velocity * 0.9f);
	//		if (pObjec1EulerMovement->velocity.y < 0.01f) {
	//			pObjec1EulerMovement->velocity.y = 0.0f;
	//		}

	//		pObjec1EulerMovement->velocity -= reflected;
	//		
	//		return true;
	//	}

	//	return false;
	//}

	return false;
}

bool cColliderSystem::testForCollisionWithStaticObject(cEntity* entity) {
	
	//not static objects loaded
	if(this->staticAABBTriangles == nullptr) {
		return false;
	}

	//draw the collision points
	drawColliders(entity);

	cTransformComponent* transformComponent = (cTransformComponent*)entity->components[1];
	cCollider* collider = (cCollider*)entity->components[5];

	//for each of the colliders do a test

	//debug rendering
	cTransformComponent* transformComponentSphere = static_cast<cTransformComponent*>(this->pColliderSphere->components[1]);
	cRenderMeshComponent* meshComponentSphere = static_cast<cRenderMeshComponent*>(this->pColliderSphere->components[3]);
	bool hitTriangle = false;
	
	for (cCollider::sPhysicsObject& x : collider->physics_objects) {
	//	
	//	//calc position of the physics object
	//	glm::vec4 vecForwardDirection_ModelSpace = glm::vec4(0.0f, 0.0f, /**/1.0f/**/, 1.0f);

	//	// Now orientation
	//	glm::quat qMig29Rotation = transformComponent->getQOrientation();
	//	// Make a mat4x4 from that quaternion
	//	glm::mat4 matQMig29rotation = glm::mat4(qMig29Rotation);

	//	glm::vec4 vecForwardDirection_WorldSpace = matQMig29rotation * vecForwardDirection_ModelSpace;

	//	// optional normalize
	//	vecForwardDirection_WorldSpace = glm::normalize(vecForwardDirection_WorldSpace);
	//	//TODO: can use this to control the plane (accel/vel)

	//	glm::vec4 contactPoint_ModelSpace = glm::vec4(x.offsetFromOrigin, 1.0f);

	//	glm::mat4 matTransform = glm::mat4(1.0f);	// identity matrix

	//	glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f),
	//		transformComponent->get_position());

	//	matTransform = matTransform * matTranslation;		// matMove

	//	// Now orientation
	//	glm::quat qRotation = transformComponent->getQOrientation();
	//	// Make a mat4x4 from that quaternion
	//	glm::mat4 matQrotation = glm::mat4(qRotation);

	//	matTransform = matTransform * matQrotation;

	//	// Calcualte the collision points
	//	glm::vec4 contactPoint_WorldSpace = glm::vec4(0.0f);
	//	contactPoint_WorldSpace = matTransform * contactPoint_ModelSpace;

	//	//get the aabb it is in
	//	unsigned long long aabbId = cAABBTerrianBroadPhase::generateId(cAABBTerrianBroadPhase::getCenterFromPosition(contactPoint_WorldSpace, AABB_HEIRARCHY_HALF_LENGTH));

	//	//check the aabb exists
	//	std::map< cAABBTerrianBroadPhase::id_type, cAABBTerrianBroadPhase* >::iterator aabbIt = this->staticAABBTriangles->m_mapAABBs.find(aabbId);
	//	if (aabbIt != this->staticAABBTriangles->m_mapAABBs.end()) {
	//		cTransformComponent* transformComponentCube = static_cast<cTransformComponent*>(this->pAABBCube->components[1]);
	//		cRenderMeshComponent* meshComponentCube = static_cast<cRenderMeshComponent*>(this->pAABBCube->components[3]);

	//		//highlight the aabb we are testing
	//		transformComponentCube->scale = glm::vec3(AABB_HEIRARCHY_HALF_LENGTH);
	//		meshComponentCube->objColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	//		transformComponentCube->set_position((*aabbIt).second->center);
	//		meshComponentCube->bIsVisible = true;
	//		glm::mat4 matIdentity(1.0f);
	//		cMeshRenderSystem::getInstance()->drawObject(this->pAABBCube, matIdentity, cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader"), 1);
	//		meshComponentCube->bIsVisible = false;
	//		
	//		//test all the triangles in the aabb
	//		std::vector<sAABB_Triangle>* trisToTest = &this->staticAABBTriangles->m_mapAABBs[aabbId]->triangles;

	//		hitTriangle = false;
	//		std::vector<sAABB_Triangle*> triangleIHit;
	//		
	//		for(int i=0; i < (int)trisToTest->size(); ++i) {
	//			sAABB_Triangle& tri = trisToTest->operator[](i);

	//			if (cPhysics::testSphereTriangle(contactPoint_WorldSpace, ((sSphere*)x.theShape)->radius, &tri)) {
	//				hitTriangle = true;
	//				triangleIHit.push_back(&tri);
	//			}
	//		}

	//		//draw the sphere green/red (red == collision)
	//		transformComponentSphere->scale = glm::vec3(((sSphere*)x.theShape)->radius);
	//		meshComponentSphere->objColour = hitTriangle ? glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) : glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	//		transformComponentSphere->set_position(contactPoint_WorldSpace);
	//		meshComponentSphere->bIsVisible = true;
	//		matIdentity = glm::mat4(1.0f);
	//		cMeshRenderSystem::getInstance()->drawObject(this->pColliderSphere, matIdentity, cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader"), 1);
	//		meshComponentSphere->bIsVisible = false;

	//		//respond to the collision
	//		for(int i=0; i < triangleIHit.size(); ++i) {
	//			
	//			glm::vec3 distanceToTri = glm::vec3(contactPoint_WorldSpace) - triangleIHit[i]->verts[0];
	//			glm::vec3 directionOfCollision = glm::normalize(distanceToTri);
	//			glm::vec3 normals = glm::abs(this->getAverageNormalForTri(*triangleIHit[i]));
	//			glm::vec3 changeInVelocity = glm::mix(glm::vec3(1.0f), glm::vec3(0.0f), normals);

	//			if(changeInVelocity.y < ZERO_VEL_RANGE && changeInVelocity.y > -ZERO_VEL_RANGE) {
	//				((cEulerMovementComponent*)entity->components[2])->velocity.y = 0;
	//			}

	//			if (changeInVelocity.x < ZERO_VEL_RANGE && changeInVelocity.x > -ZERO_VEL_RANGE) {
	//				((cEulerMovementComponent*)entity->components[2])->velocity.x = 0;
	//			}

	//			if (changeInVelocity.z < ZERO_VEL_RANGE && changeInVelocity.z > -ZERO_VEL_RANGE) {
	//				((cEulerMovementComponent*)entity->components[2])->velocity.z = 0;
	//			}
	//		}
	//	}
	//	else {
	//		
	//		//draw the sphere green (nothing to test)
	//		transformComponentSphere->scale = glm::vec3(((sSphere*)x.theShape)->radius);
	//		meshComponentSphere->objColour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	//		transformComponentSphere->set_position(contactPoint_WorldSpace);
	//		meshComponentSphere->bIsVisible = true;
	//		glm::mat4 matIdentity(1.0f);
	//		cMeshRenderSystem::getInstance()->drawObject(this->pColliderSphere, matIdentity, cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader"),1 );
	//		meshComponentSphere->bIsVisible = false;
	//	}
	}

	return false;
}

void cColliderSystem::drawColliders(cEntity* entity) {

	//draw the debug spheres on the model
	cEntity* pControlledEntity = entity;
	if (pControlledEntity->components[5] != nullptr)
	{
		cCollider* collider = (cCollider*)pControlledEntity->components[5];
		cTransformComponent* transformComponent = static_cast<cTransformComponent*>(pControlledEntity->components[1]);
		cRenderMeshComponent* meshComponent = static_cast<cRenderMeshComponent*>(pControlledEntity->components[3]);

		for (cCollider::sPhysicsObject x : collider->physics_objects) {
			glm::vec4 vecForwardDirection_ModelSpace = glm::vec4(0.0f, 0.0f, /**/1.0f/**/, 1.0f);

			// Now orientation
			glm::quat qMig29Rotation = transformComponent->getQOrientation();
			// Make a mat4x4 from that quaternion
			glm::mat4 matQMig29rotation = glm::mat4(qMig29Rotation);

			glm::vec4 vecForwardDirection_WorldSpace = matQMig29rotation * vecForwardDirection_ModelSpace;

			// optional normalize
			vecForwardDirection_WorldSpace = glm::normalize(vecForwardDirection_WorldSpace);
			//TODO: can use this to control the plane (accel/vel)

			glm::vec4 contactPoint_ModelSpace = glm::vec4(x.offsetFromOrigin, 1.0f);

			// These transformations are taken from the draw call. 
			// We want them to be EXACTLY THE SAME, same order, etc.

			glm::mat4 matTransform = glm::mat4(1.0f);	// identity matrix

			glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f),
				transformComponent->get_position());

			matTransform = matTransform * matTranslation;		// matMove

			// Now orientation
			glm::quat qRotation = transformComponent->getQOrientation();
			// Make a mat4x4 from that quaternion
			glm::mat4 matQrotation = glm::mat4(qRotation);

			matTransform = matTransform * matQrotation;

			// Calcualte the collision points
			glm::vec4 contactPoint_WorldSpace = glm::vec4(0.0f);
			contactPoint_WorldSpace = matTransform * contactPoint_ModelSpace;

			// In this case, we will draw them as spheres
			// But you would pass these "world space" points into the collision test...
			cTransformComponent* transformComponentSphere = static_cast<cTransformComponent*>(this->pColliderSphere->components[1]);
			cRenderMeshComponent* meshComponentSphere = static_cast<cRenderMeshComponent*>(this->pColliderSphere->components[3]);

			glm::vec3 oldPosition = transformComponentSphere->get_position();
			glm::vec3 oldScale = transformComponentSphere->scale;

			meshComponentSphere->bIsVisible = true;
			meshComponentSphere->bIsWireFrame = true;
			meshComponentSphere->bDontLight = true;
			transformComponentSphere->scale = glm::vec3(0.5f);

			glm::mat4 matIdentity = glm::mat4(1.0f);
			
		}
	}
}

glm::vec3 cColliderSystem::getAverageNormalForTri(sAABB_Triangle tri) {
	
	glm::vec3 norm = glm::cross((tri.verts[0] - tri.verts[1]), (tri.verts[1] - tri.verts[2])); //perform cross product of two lines on plane
	
	return glm::normalize(norm);
}
