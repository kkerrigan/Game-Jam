#include "cAssimpBasic.h"
#include "cAssimpBasic_Imp.h"

cAssimpBasic::cAssimpBasic() {
	
	this->m_pImp = new cAssimpBasic_Imp();

	return;
}

cAssimpBasic::~cAssimpBasic() {
	
	delete this->m_pImp;

	return;
}

// Loads a model and returns a mesh
bool cAssimpBasic::loadModelA(std::string filename, cMesh &theMesh, std::string &error) {

	return this->m_pImp->loadModelA( filename, theMesh, error );
}
