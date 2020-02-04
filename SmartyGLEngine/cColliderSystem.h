#ifndef _cColliderSystem_HG_
#define _cColliderSystem_HG_

#include <vector>

#include "iSystem.h"
#include "physicsShapes.h"
#include "cMeshRenderSystem.h"
#include "cSceneManager.h"
#include "cCollider.h"
#include "cShaderManager.h"

class cColliderSystem : public iSystem {
public:

	static cColliderSystem* getInstance();
	cColliderSystem(cColliderSystem const&) = delete;
	void operator=(cColliderSystem const&) = delete;

	void update(double deltaTime);
	void registerEntity(cEntity* entity);
	void unregisterEntity(cEntity* entity);

	void setStaticObjectsInfo(cAABBTerrianBroadPhaseHierarchy* ptr);

	cEntity* pColliderSphere;
	cEntity* pAABBCube;


private:
	cColliderSystem(): staticAABBTriangles(nullptr), pColliderSphere(nullptr), pAABBCube(nullptr){}
	~cColliderSystem() {}

	std::vector<cEntity*> entitiesToUpdate;
	cAABBTerrianBroadPhaseHierarchy* staticAABBTriangles;

	bool testForCollision(cEntity* entity1, cEntity* entity2, double deltaTime);
	bool testForCollisionWithStaticObject(cEntity* entity);

	void drawColliders(cEntity* entity);

	glm::vec3 getAverageNormalForTri(sAABB_Triangle tri);
};

#endif
