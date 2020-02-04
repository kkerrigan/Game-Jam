#ifndef _physicsShapes_HG_
#define _physicsShapes_HG_

#define AABB_HEIRARCHY_HALF_LENGTH 2.50f

#include <glm/glm.hpp>
#include <map>
#include <vector>

struct sClosestPointData {
	glm::vec3 thePoint;
	unsigned int triangleIndex;
};

struct sSphere {
	sSphere(float theRadius)
	{
		this->radius = theRadius;
	}
	float radius;
};

struct sTriangle {
	glm::vec3 v[3];
};

struct sAABB
{
	glm::vec3 min;
	glm::vec3 max;
};

struct sAABB_Triangle {
	// What information do you want to store here...? 
	glm::vec3 verts[3];
	glm::vec3 norms[3];
};

class cAABBTerrianBroadPhase {
public:
	typedef unsigned long long id_type;

	cAABBTerrianBroadPhase();
	cAABBTerrianBroadPhase(glm::vec3 center, float halfLength);

	id_type id;
	glm::vec3 center;
	float halfLength;
	std::vector<sAABB_Triangle> triangles;

	// getters
	glm::vec3 getMinXYZ();
	float getSideLength();
	static glm::vec3 getCenterFromPosition(glm::vec3 pos, float halfLength);

	// generators
	static id_type generateId(glm::vec3 center);
	static id_type generateIdFromPosition(glm::vec3 pos, float halfLength);
};

class cAABBTerrianBroadPhaseHierarchy {
public:		
	std::map< cAABBTerrianBroadPhase::id_type, cAABBTerrianBroadPhase* > m_mapAABBs;
};

#endif
