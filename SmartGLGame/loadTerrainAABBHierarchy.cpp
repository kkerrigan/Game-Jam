#include <physicsShapes.h>
#include <cVAOMeshManager.h>
#include "cColliderSystem.h"
#include "cSceneManager.h"

cAABBTerrianBroadPhaseHierarchy g_AABBTerrianHierarchy;

glm::vec3 converVertsToWorld(glm::vec3 point, glm::vec3 position, glm::quat orientation)
{
	glm::vec4 contactPoint_ModelSpace = glm::vec4(point, 1.0f);
	glm::mat4 matTransform = glm::mat4(1.0f);	// identity matrix
	glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f),position);
	matTransform = matTransform * matTranslation;		// matMove

	// Now orientation
	glm::quat qRotation = orientation;
	// Make a mat4x4 from that quaternion
	glm::mat4 matQrotation = glm::mat4(qRotation);

	matTransform = matTransform * matQrotation;

	// Calcualte the collision points
	glm::vec4 contactPoint_WorldSpace = glm::vec4(0.0f);
	return  matTransform * contactPoint_ModelSpace;


}

void addVertToMap(sAABB_Triangle& curTri)
{
	for (unsigned int vertIndex = 0; vertIndex != 3; vertIndex++)
	{
		for(int j=0; j<3;++j)
		{
			// What AABB is "this" vertex in? 
			unsigned long long AABB_ID = cAABBTerrianBroadPhase::generateIdFromPosition(curTri.verts[j], AABB_HEIRARCHY_HALF_LENGTH);

			// Do we have this AABB alredy? 
			std::map< unsigned long long/*ID AABB*/, cAABBTerrianBroadPhase* >::iterator itAABB = g_AABBTerrianHierarchy.m_mapAABBs.find(AABB_ID);
			if (itAABB == g_AABBTerrianHierarchy.m_mapAABBs.end())
			{
				// We DON'T have an AABB, yet

				// Determine the AABB location for this point
				// (like the generateID() method...)
				// get the center 

				glm::vec3 center = cAABBTerrianBroadPhase::getCenterFromPosition(curTri.verts[j], AABB_HEIRARCHY_HALF_LENGTH);

				cAABBTerrianBroadPhase* pAABB = new cAABBTerrianBroadPhase(center, AABB_HEIRARCHY_HALF_LENGTH);
				g_AABBTerrianHierarchy.m_mapAABBs[AABB_ID] = pAABB;

				// Then set the iterator to the AABB, by running find again
				itAABB = g_AABBTerrianHierarchy.m_mapAABBs.find(AABB_ID);

				itAABB->second->triangles.push_back(curTri);
			}
		}
	}
}

void loadTerrainIntoTerrainAABBHierarchy(cEntity* staticEntity) {
	cTransformComponent* transformComp = (cTransformComponent*)staticEntity->components[1];
	cRenderMeshComponent* meshComp = (cRenderMeshComponent*)staticEntity->components[3];

	sModelDrawInfo terrainMeshInfo;
	terrainMeshInfo.meshFileName = meshComp->meshName;

	cVAOMeshManager::getInstance()->FindDrawInfoByModelName(terrainMeshInfo);
	// How big is our AABBs? Side length?
	//float sideLength = AABB_HEIRARCHY_HALF_LENGTH * 2;		

	for (unsigned int triIndex = 0; triIndex != terrainMeshInfo.numberOfTriangles; triIndex++)
	{
		// for each triangle, for each vertex, determine which AABB the triangle should be in
		// (if your mesh has been transformed, then you need to transform the tirangles 
		//  BEFORE you do this... or just keep the terrain UNTRANSFORMED)
		sPlyTriangle currentTri = terrainMeshInfo.pTriangles[triIndex];
		sPlyVertex currentVerts[3];
		currentVerts[0] = terrainMeshInfo.pVerticesFromFile[currentTri.vertex_index_1];
		currentVerts[1] = terrainMeshInfo.pVerticesFromFile[currentTri.vertex_index_2];
		currentVerts[2] = terrainMeshInfo.pVerticesFromFile[currentTri.vertex_index_3];

		//TODO:move the vertexes into world space

		// This is the structure we are eventually going to store in the AABB map...
		sAABB_Triangle curAABBTri;

		//verts
		curAABBTri.verts[0].x = currentVerts[0].x;
		curAABBTri.verts[0].y = currentVerts[0].y;
		curAABBTri.verts[0].z = currentVerts[0].z;
		curAABBTri.verts[1].x = currentVerts[1].x;
		curAABBTri.verts[1].y = currentVerts[1].y;
		curAABBTri.verts[1].z = currentVerts[1].z;
		curAABBTri.verts[2].x = currentVerts[2].x;
		curAABBTri.verts[2].y = currentVerts[2].y;
		curAABBTri.verts[2].z = currentVerts[2].z;

		curAABBTri.verts[0] = converVertsToWorld(curAABBTri.verts[0], transformComp->get_position(), transformComp->getQOrientation());
		curAABBTri.verts[1] = converVertsToWorld(curAABBTri.verts[1], transformComp->get_position(), transformComp->getQOrientation());
		curAABBTri.verts[2] = converVertsToWorld(curAABBTri.verts[2], transformComp->get_position(), transformComp->getQOrientation());

		//normals
		curAABBTri.norms[0].x = currentVerts[0].nx;
		curAABBTri.norms[0].y = currentVerts[0].ny;
		curAABBTri.norms[0].z = currentVerts[0].nz;
		curAABBTri.norms[1].x = currentVerts[1].nx;
		curAABBTri.norms[1].y = currentVerts[1].ny;
		curAABBTri.norms[1].z = currentVerts[1].nz;
		curAABBTri.norms[2].x = currentVerts[2].nx;
		curAABBTri.norms[2].y = currentVerts[2].ny;
		curAABBTri.norms[2].z = currentVerts[2].nz;

		addVertToMap(curAABBTri);
	}
}

void loadTerrainIntoTerrainAABBHierarchy(std::string meshName) {
	sModelDrawInfo terrainMeshInfo;
	terrainMeshInfo.meshFileName = meshName;

	cVAOMeshManager::getInstance()->FindDrawInfoByModelName(terrainMeshInfo);
	// How big is our AABBs? Side length?
	//float sideLength = AABB_HEIRARCHY_HALF_LENGTH * 2;		

	for (unsigned int triIndex = 0; triIndex != terrainMeshInfo.numberOfTriangles; triIndex++)
	{
		// for each triangle, for each vertex, determine which AABB the triangle should be in
		// (if your mesh has been transformed, then you need to transform the tirangles 
		//  BEFORE you do this... or just keep the terrain UNTRANSFORMED)
		sPlyTriangle currentTri = terrainMeshInfo.pTriangles[triIndex];
		sPlyVertex currentVerts[3];
		currentVerts[0] = terrainMeshInfo.pVerticesFromFile[currentTri.vertex_index_1];
		currentVerts[1] = terrainMeshInfo.pVerticesFromFile[currentTri.vertex_index_2];
		currentVerts[2] = terrainMeshInfo.pVerticesFromFile[currentTri.vertex_index_3];
		
		// This is the structure we are eventually going to store in the AABB map...
		sAABB_Triangle curAABBTri;

		//verts
		curAABBTri.verts[0].x = currentVerts[0].x;
		curAABBTri.verts[0].y = currentVerts[0].y;
		curAABBTri.verts[0].z = currentVerts[0].z;
		curAABBTri.verts[1].x = currentVerts[1].x;
		curAABBTri.verts[1].y = currentVerts[1].y;
		curAABBTri.verts[1].z = currentVerts[1].z;
		curAABBTri.verts[2].x = currentVerts[2].x;
		curAABBTri.verts[2].y = currentVerts[2].y;
		curAABBTri.verts[2].z = currentVerts[2].z;

		//normals
		curAABBTri.norms[0].x = currentVerts[0].nx;
		curAABBTri.norms[0].y = currentVerts[0].ny;
		curAABBTri.norms[0].z = currentVerts[0].nz;
		curAABBTri.norms[1].x = currentVerts[1].nx;
		curAABBTri.norms[1].y = currentVerts[1].ny;
		curAABBTri.norms[1].z = currentVerts[1].nz;
		curAABBTri.norms[2].x = currentVerts[2].nx;
		curAABBTri.norms[2].y = currentVerts[2].ny;
		curAABBTri.norms[2].z = currentVerts[2].nz;

		//TODO:
		// Is the triangle "too big", and if so, split it (take centre and make 3 more)
		// (Pro Tip: "too big" is the SMALLEST side is greater than HALF the AABB length)
		// Use THOSE triangles as the test (and recursively do this if needed),
		// +++BUT+++ store the ORIGINAL triangle info NOT the subdivided one
		// 
		// For the student to complete... 
		// 
		addVertToMap(curAABBTri);
	}
	
	return;
}


void loadTerrainAABBHierarchy() {

	loadTerrainIntoTerrainAABBHierarchy("graphics_island.ply");

	cEntity* player = cSceneManager::getInstance()->getActiveScene()->getSelectedEntity();
	
	std::vector<cEntity*>& staticEntities = cSceneManager::getInstance()->getActiveScene()->getEntities();
	for(size_t i=0; i < staticEntities.size();++i)
	{
		if(staticEntities[i]->entityType == 21)
			loadTerrainIntoTerrainAABBHierarchy(staticEntities[i]);
	}

	cColliderSystem::getInstance()->setStaticObjectsInfo(&g_AABBTerrianHierarchy);
}