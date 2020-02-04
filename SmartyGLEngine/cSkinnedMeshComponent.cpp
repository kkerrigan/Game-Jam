#include "cSkinnedMeshComponent.h"

#include "sModelDrawInfo.h"
#include "cVAOMeshManager.h"
#include "cShaderManager.h"
#include "cMesh.h"

// c'tor
cSkinnedMeshComponent::cSkinnedMeshComponent() : cComponent(8) {

	this->pSkinnedMesh = new cSimpleAssimpSkinnedMesh();
	this->pAniState = nullptr;

	return;
}


bool cSkinnedMeshComponent::assimpToVAOConverter() {


	sModelDrawInfo drawInfo;
	drawInfo.meshFileName = this->pSkinnedMesh->fileName;
	drawInfo.friendlyName = this->pSkinnedMesh->friendlyName;

	// Copy the data from assimp format into sModelDrawInfo format
	aiMesh* curMesh = this->pSkinnedMesh->pScene->mMeshes[0];
	drawInfo.numberOfVertices = curMesh->mNumVertices;
	drawInfo.numberOfTriangles = curMesh->mNumFaces;

	// We used the "triangulate" option when loading so all the primitives will be triangles
	// but BEWARE!
	drawInfo.numberOfIndices = drawInfo.numberOfTriangles * 3;

	// Allocate the vertex array
	drawInfo.pMeshData = new cMesh();
	drawInfo.pMeshData->pVertices = new sVertex_xyz_rgba_n_uv2_bt_4Bones[drawInfo.numberOfVertices];
	memset(drawInfo.pMeshData->pVertices, 0, sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones) * drawInfo.numberOfVertices);

	for (unsigned int vertIndex = 0; vertIndex != drawInfo.numberOfVertices; ++vertIndex) {

		sVertex_xyz_rgba_n_uv2_bt_4Bones curVert = drawInfo.pMeshData->pVertices[vertIndex];

		curVert.x = curMesh->mVertices[vertIndex].x;
		curVert.y = curMesh->mVertices[vertIndex].y;
		curVert.z = curMesh->mVertices[vertIndex].z;

		// Normals
		curVert.nx = curMesh->mNormals[vertIndex].x;
		curVert.ny = curMesh->mNormals[vertIndex].y;
		curVert.nz = curMesh->mNormals[vertIndex].z;

		// Colours (if no colour make hot pink)
		if (curMesh->HasVertexColors(0)) {

			curVert.r = curMesh->mColors[vertIndex]->r;
			curVert.g = curMesh->mColors[vertIndex]->g;
			curVert.b = curMesh->mColors[vertIndex]->b;
			curVert.a = curMesh->mColors[vertIndex]->a;
		}
		else {

			curVert.r = 1.0f;
			curVert.g = 105.0f / 255.0f;
			curVert.b = 180.0f / 255.0f;
			curVert.a = 1.0f;
		}

		// Bi-normal (bi-tangent)
		curVert.bx = curMesh->mBitangents[vertIndex].x;
		curVert.by = curMesh->mBitangents[vertIndex].y;
		curVert.bz = curMesh->mBitangents[vertIndex].z;

		// Tangent
		curVert.tx = curMesh->mTangents[vertIndex].x;
		curVert.ty = curMesh->mTangents[vertIndex].y;
		curVert.tz = curMesh->mTangents[vertIndex].z;

		// UV2 
		// Note: there is an array of texture coordinates, up to 8 but usually only 1
		if (curMesh->HasTextureCoords(0)) {

			curVert.u0 = curMesh->mTextureCoords[0]->x;
			curVert.v0 = curMesh->mTextureCoords[0]->y;
		}

		if (curMesh->HasTextureCoords(1)) {

			curVert.u0 = curMesh->mTextureCoords[1]->x;
			curVert.v0 = curMesh->mTextureCoords[1]->y;
		}

		

		// 4Bones: ids and weights
		curVert.boneID[0] = this->pSkinnedMesh->vecVertexBoneData[vertIndex].ids[0];
		curVert.boneID[1] = this->pSkinnedMesh->vecVertexBoneData[vertIndex].ids[1];
		curVert.boneID[2] = this->pSkinnedMesh->vecVertexBoneData[vertIndex].ids[2];
		curVert.boneID[3] = this->pSkinnedMesh->vecVertexBoneData[vertIndex].ids[3];

		curVert.boneWeights[0] = this->pSkinnedMesh->vecVertexBoneData[vertIndex].weights[0];
		curVert.boneWeights[1] = this->pSkinnedMesh->vecVertexBoneData[vertIndex].weights[1];
		curVert.boneWeights[2] = this->pSkinnedMesh->vecVertexBoneData[vertIndex].weights[2];
		curVert.boneWeights[3] = this->pSkinnedMesh->vecVertexBoneData[vertIndex].weights[3];
	} // for (unsigned int vertIndex = 0;

	// Allocate the array to hold the indices (triangle) info
	drawInfo.pMeshData->pIndices = new unsigned int[drawInfo.numberOfIndices];
	memset(drawInfo.pMeshData->pIndices, 0, sizeof(unsigned int) * drawInfo.numberOfIndices);

	unsigned int numTriangles = curMesh->mNumFaces;
	unsigned int triIndex = 0; // Steps through the triangles
	unsigned int indicesIndex = 0; // Steps through the indicies (index buffer)

	for (; triIndex != drawInfo.numberOfTriangles; ++triIndex, indicesIndex += 3) {

		// Get the triangle at the current index
		aiFace* pCurFace = &(curMesh->mFaces[triIndex]);
		drawInfo.pMeshData->pIndices[indicesIndex + 0] = pCurFace->mIndices[0];
		drawInfo.pMeshData->pIndices[indicesIndex + 1] = pCurFace->mIndices[1];
		drawInfo.pMeshData->pIndices[indicesIndex + 2] = pCurFace->mIndices[2];
	}

	// Calculate the extents on the mesh
	// TODO: Fix duplication of code
	drawInfo.pMeshData->name = drawInfo.friendlyName;
	drawInfo.pMeshData->numberOfIndices = drawInfo.numberOfIndices;
	drawInfo.pMeshData->numberOfTriangles = drawInfo.numberOfTriangles;
	drawInfo.pMeshData->numberOfVertices = drawInfo.numberOfVertices;

	drawInfo.pMeshData->calculateExtents();

	// Pass it to the mesh manager
	cVAOMeshManager::getInstance()->LoadModelIntoVAO(drawInfo, cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader"));


	return true;
}