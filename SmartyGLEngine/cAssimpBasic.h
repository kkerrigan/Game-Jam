#ifndef _cAssimpBasic_HG_
#define _cAssimpBasic_HG_

class cAssimpBasic_Imp;
#include "cMesh.h"


class cAssimpBasic {
public:
	cAssimpBasic();
	~cAssimpBasic();

	// Loads a model and returns a mesh
	bool loadModelA(std::string filename, cMesh &theMesh, std::string &error);

private:
	cAssimpBasic_Imp* m_pImp;
};

#endif
