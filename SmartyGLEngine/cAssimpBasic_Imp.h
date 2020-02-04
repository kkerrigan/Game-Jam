#ifndef _cAssimpBasic_Imp_HG_
#define _cAssimpBasic_Imp_HG_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "cMesh.h"

class cAssimpBasic_Imp {
public:
	cAssimpBasic_Imp();
	~cAssimpBasic_Imp();

	// Loads a model and returns a mesh
	bool loadModelA(std::string filename, cMesh &theMesh, std::string &error);
};

#endif 

