#ifndef _cPhysics_HG_
#define _cPhysics_HG_

#include <vector>
#include <glm/glm.hpp>

#include "sModelDrawInfo.h"
#include "cEntity.h"
#include "physicsShapes.h"
#include "cCollider.h"

class cPhysics {
public:
	typedef glm::vec3 Point;
	typedef glm::vec3 Vector;

	static Point closestPtPointTriangle(Point p, Point a, Point b, Point c);
	static void calculateClosestPointsOnMesh(sModelDrawInfo theMeshDrawInfo, glm::vec3 pointToTest, std::vector<sClosestPointData> &vecPoints);
	static bool areSpheresPenetrating(cEntity* entity1, cEntity* entity2);
	static void closestPtPointAABB(glm::vec3 p, sAABB b, glm::vec3 &q);
	static bool testSphereAABB(cEntity* s, cEntity* b, glm::vec3 &p);
	static bool testInSphereAABB(cEntity* s, cEntity* b, glm::vec3 &p);
	static bool sphereTraingleTest(cEntity* pSphere, sTriangle* pTriangle, cEntity* triOwner);


	static bool testSphereTriangle(glm::vec3 sphereWorldPos, float sphereRadius, sAABB_Triangle* triangle);
};

#endif
