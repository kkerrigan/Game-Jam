#include "cPhysics.h"

#include "cTransformComponent.h"
#include "cEulerMovementComponent.h"
#include "cCollider.h"

cPhysics::Point cPhysics::closestPtPointTriangle(cPhysics::Point p, cPhysics::Point a, cPhysics::Point b, cPhysics::Point c) {

	// Check if P in vertex region outside of A
	cPhysics::Vector ab = b - a;
	cPhysics::Vector ac = c - a;
	cPhysics::Vector ap = p - a;
	float d1 = glm::dot(ab, ap);
	float d2 = glm::dot(ac, ap);
	if (d1 <= 0.0f && d2 <= 0.0f) return a; // barycentric coordinates (1,0,0)

	// Check if P in vertex region outside of B
	cPhysics::Vector bp = p - b;
	float d3 = glm::dot(ab, bp);
	float d4 = glm::dot(ac, bp);
	if (d3 >= 0.0f && d4 <= d3) return b; // barycentric coordinates (0,1,0)

	// Check if P in edge region of AB, if so return projection of P onto AB
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
		float v = d1 / (d1 - d3);
		return a + v * ab; // barycentric coordinates (1-v,v,0)
	}

	// Check if P in vertex region outside C
	cPhysics::Vector cp = p - c;
	float d5 = glm::dot(ab, cp);
	float d6 = glm::dot(ac, cp);
	if (d6 >= 0.0f && d5 <= d6) return c; // barycentric coordinate (0.0.1)

	// Check if P in edge region of AC, if so return projection of P onto AC
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
		float w = d2 / (d2 - d6);
		return a + w * ac; // barycentric coordinates (1-w,0,w)
	}

	// Check if P in edge region of BC, if so return projection of P onto BC
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return b + w * (c - b); // barycentric coordinates (0,1-w,w)
	}

	// P inside face region. Compute Q through its barycentric coordinates (u,v,w)
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	return a + ab * v + ac * w; // = u*a + v*b + w*c, u = va*denom = 1.0f - v - w
}

void cPhysics::calculateClosestPointsOnMesh(sModelDrawInfo theMeshDrawInfo, glm::vec3 pointToTest, std::vector<sClosestPointData> &vecPoints) {

	vecPoints.clear();

	// For each triangle in the mesh information...
	for (unsigned int triIndex = 0; triIndex != theMeshDrawInfo.numberOfTriangles; triIndex++)
	{
		sPlyTriangle CurTri = theMeshDrawInfo.pTriangles[triIndex];

		sClosestPointData closestTri;
		closestTri.triangleIndex = triIndex;

		// ... call the ClosestPointToTriangle...
		// Need to get the 3 vertices of the triangle
		sPlyVertex corner_1 = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_1];
		sPlyVertex corner_2 = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_2];
		sPlyVertex corner_3 = theMeshDrawInfo.pVerticesFromFile[CurTri.vertex_index_3];

		// Convert this to glm::vec3
		glm::vec3 vert_1 = glm::vec3(corner_1.x, corner_1.y, corner_1.z);
		glm::vec3 vert_2 = glm::vec3(corner_2.x, corner_2.y, corner_2.z);
		glm::vec3 vert_3 = glm::vec3(corner_3.x, corner_3.y, corner_3.z);

		closestTri.thePoint = cPhysics::closestPtPointTriangle(pointToTest, vert_1, vert_2, vert_3);

		vecPoints.push_back(closestTri);

		// ...and push back the restulting point
	}//for ( unsigned int triIndex = 0

	return;
}

bool cPhysics::areSpheresPenetrating(cEntity* entity1, cEntity* entity2) {

	cTransformComponent* pObj1Transform = static_cast<cTransformComponent*>(entity1->components[1]);
	cEulerMovementComponent* pObjec1EulerMovement = static_cast<cEulerMovementComponent*>(entity1->components[2]);
	cCollider* pObj1Collider = static_cast<cCollider*>(entity1->components[5]);

	cTransformComponent* pObj2Transform = static_cast<cTransformComponent*>(entity2->components[1]);
	cEulerMovementComponent* pObjec2EulerMovement = static_cast<cEulerMovementComponent*>(entity2->components[2]);
	cCollider* pObj2Collider = static_cast<cCollider*>(entity2->components[5]);

	//sSphere* pSphereA = (sSphere*)(pObj1Collider->pTheShape);
	//sSphere* pSphereB = (sSphere*)(pObj2Collider->pTheShape);

	//if (glm::distance(pObj1Transform->position, pObj2Transform->position) < (pSphereA->radius + pSphereB->radius)) {
	//	// Yup
	//	return true;
	//}
	//// Nope 
	return false;
}

void cPhysics::closestPtPointAABB(glm::vec3 p, sAABB b, glm::vec3 &q) {

	// For each coordinate axis, if the point coordinate value is
	// outside box, clamp it to the box, else keep it as is
	for (int i = 0; i < 3; i++) {
		float v = p[i];
		if (v < b.min[i]) v = b.min[i]; // v = max(v, b.min[i])
		if (v > b.max[i]) v = b.max[i]; // v = min(v, b.max[i])
		q[i] = v;
	}

	return;
}

bool cPhysics::testSphereAABB(cEntity* s, cEntity* b, glm::vec3 &p) {

	cTransformComponent* pObj1Transform = static_cast<cTransformComponent*>(s->components[1]);
	cEulerMovementComponent* pObjec1EulerMovement = static_cast<cEulerMovementComponent*>(s->components[2]);
	cCollider* pObj1Collider = static_cast<cCollider*>(s->components[5]);

	cTransformComponent* pObj2Transform = static_cast<cTransformComponent*>(b->components[1]);
	cEulerMovementComponent* pObjec2EulerMovement = static_cast<cEulerMovementComponent*>(b->components[2]);
	cCollider* pObj2Collider = static_cast<cCollider*>(b->components[5]);

	//sSphere* pSphereA = (sSphere*)(pObj1Collider->pTheShape);
	//sAABB* pAABB = (sAABB*)(pObj2Collider->pTheShape);

	//// Find point p on AABB closest to sphere center
	//cPhysics::closestPtPointAABB(pObj1Transform->position, *pAABB, p);

	//// Sphere and AABB intersect if the (squared) distance from sphere
	//// center to point p is less than the (squared) sphere radius
	//glm::vec3 v = p - pObj1Transform->position;
	//return glm::dot(v, v) <= pSphereA->radius * pSphereA->radius;

	return false;
}

bool cPhysics::testInSphereAABB(cEntity* s, cEntity* b, glm::vec3 &p) {
	return !cPhysics::testSphereAABB(s, b, p);
}

bool cPhysics::sphereTraingleTest(cEntity* pSphere, sTriangle* pTriangle, cEntity* triOwner) {

	//sSphere* pSphereA = (sSphere*)((cCollider*)pSphere->components[4])->pTheShape;
	//sTriangle* pTri = pTriangle;

	//// Calculate where the triangle vertices ACTUALLY are...
	//// Calculating based on WORLD position
	//glm::vec3 vActual[3];

	//glm::mat4 matWorld = glm::mat4(1.0f);

	//// Take into account the rotation in the world
	//glm::mat4 matQrotation = glm::mat4(((cTransformComponent*)triOwner->components[1])->orientation);

	//glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f), ((cTransformComponent*)triOwner->components[1])->position); // Move it

	//// Get the "final" world matrix
	//matWorld = matWorld * matTranslate;

	//matWorld = matWorld * matQrotation;

	//vActual[0] = matWorld * glm::vec4(pTri->v[0], 1.0f);
	//vActual[1] = matWorld * glm::vec4(pTri->v[1], 1.0f);
	//vActual[2] = matWorld * glm::vec4(pTri->v[2], 1.0f);

	//glm::vec3 closestPointToTri = cPhysics::closestPtPointTriangle(((cTransformComponent*)pSphere->components[1])->position, vActual[0], vActual[1], vActual[2]);

	//// is this point LESS THAN the radius of the sphere? 
	//if (glm::distance(closestPointToTri, ((cTransformComponent*)pSphere->components[1])->position) <= (pSphereA->radius + 0.5f)) {
	//	return true;
	//}

	return false;
}

bool cPhysics::testSphereTriangle(glm::vec3 sphereWorldPos, float sphereRadius, sAABB_Triangle* triangle)
{
	glm::vec3 closestPointToTri = cPhysics::closestPtPointTriangle(sphereWorldPos, triangle->verts[0], triangle->verts[1], triangle->verts[2]);

	return glm::abs(glm::distance(closestPointToTri, sphereWorldPos)) <= (sphereRadius +1.5f);
}

