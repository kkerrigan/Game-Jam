#include "cAssimpBasic_Imp.h"

cAssimpBasic_Imp::cAssimpBasic_Imp() {
	
	return;
}

cAssimpBasic_Imp::~cAssimpBasic_Imp() {
	
	return;
}



// Loads a model and returns a mesh
bool cAssimpBasic_Imp::loadModelA(std::string filename, cMesh &theMesh, std::string &error) {
	
	const struct aiScene* scene = NULL;

	Assimp::Importer mImporter;
	unsigned int Flags = aiProcess_Triangulate | 
		                 aiProcess_OptimizeMeshes | 
		                 aiProcess_OptimizeGraph | 
		                 aiProcess_JoinIdenticalVertices | 
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace;

	scene = mImporter.ReadFile(filename.c_str(), Flags);

	if (scene->HasMeshes()) {
		
		// assume it's the 1st mesh only
		theMesh.numberOfVertices = scene->mMeshes[0]->mNumVertices;

		// Create the number of vertices that this mesh has.
		//theMesh.pVertices = new sVertex_xyz_rgba_n_uv2_bt[theMesh.numberOfVertices];
		theMesh.pVertices = new sVertex_xyz_rgba_n_uv2_bt_4Bones[theMesh.numberOfVertices];

		theMesh.minXYZ.x = scene->mMeshes[0]->mVertices[0].x;
		theMesh.minXYZ.y = scene->mMeshes[0]->mVertices[0].y;
		theMesh.minXYZ.z = scene->mMeshes[0]->mVertices[0].z;

		theMesh.maxXYZ = theMesh.minXYZ;


		for (unsigned int index = 0; index != theMesh.numberOfVertices; index++ ) {
			
			theMesh.pVertices[index].x = scene->mMeshes[0]->mVertices[index].x;
			theMesh.pVertices[index].y = scene->mMeshes[0]->mVertices[index].y;
			theMesh.pVertices[index].z = scene->mMeshes[0]->mVertices[index].z;

			if (theMesh.pVertices[index].x < theMesh.minXYZ.x)	{ theMesh.minXYZ.x = theMesh.pVertices[index].x; }
			if (theMesh.pVertices[index].y < theMesh.minXYZ.y)	{ theMesh.minXYZ.y = theMesh.pVertices[index].y; }
			if (theMesh.pVertices[index].z < theMesh.minXYZ.z)	{ theMesh.minXYZ.z = theMesh.pVertices[index].z; }

			if (theMesh.pVertices[index].x > theMesh.maxXYZ.x)	{ theMesh.maxXYZ.x = theMesh.pVertices[index].x; }
			if (theMesh.pVertices[index].y > theMesh.maxXYZ.y)	{ theMesh.maxXYZ.y = theMesh.pVertices[index].y; }
			if (theMesh.pVertices[index].z > theMesh.maxXYZ.z)	{ theMesh.maxXYZ.z = theMesh.pVertices[index].z; }


			theMesh.pVertices[index].nx = scene->mMeshes[0]->mNormals[index].x;
			theMesh.pVertices[index].ny = scene->mMeshes[0]->mNormals[index].y;
			theMesh.pVertices[index].nz = scene->mMeshes[0]->mNormals[index].z;

			// A little crazy... 
			// - There can be up to 8 UVs PER vertex
			// - [0] is the 1st UV for that vertex
			// - 2 is that there are 2 UV COORDINATES at THAT vertex (u & v ..... or s&t)
			if (scene->mMeshes[0]->mNumUVComponents[0] == 2 ) {
				theMesh.pVertices[index].u1 = scene->mMeshes[0]->mTextureCoords[0][index].x;
				theMesh.pVertices[index].v1 = scene->mMeshes[0]->mTextureCoords[0][index].y;

//				theMesh.pVertices[index].u2 = scene->mMeshes[0]->mTextureCoords[1][index].x;
//				theMesh.pVertices[index].v2 = scene->mMeshes[0]->mTextureCoords[1][index].y;
			}
		}//for ( unsigned int index

		theMesh.numberOfTriangles = scene->mMeshes[0]->mNumFaces;
		theMesh.pTriangles = new cTriangle[theMesh.numberOfTriangles];

		// Now copy the triangle vertex information (indices)
		for (int triIndex = 0; triIndex != theMesh.numberOfTriangles; triIndex++) {
			
			theMesh.pTriangles[triIndex].vertex_ID_0 
				= scene->mMeshes[0]->mFaces[triIndex].mIndices[0];

			theMesh.pTriangles[triIndex].vertex_ID_1 
				= scene->mMeshes[0]->mFaces[triIndex].mIndices[1];

			theMesh.pTriangles[triIndex].vertex_ID_2 
				= scene->mMeshes[0]->mFaces[triIndex].mIndices[2];
		}//for ( int triIndex...


		theMesh.name = filename;

		theMesh.calculateExtents();
		theMesh.scaleForUnitBBox = 1.0f / theMesh.maxExtent;

	}//if ( scene->HasMeshes() )

	return true;
}



