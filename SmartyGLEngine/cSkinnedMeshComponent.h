#ifndef _cSkinnedMeshComponent_HG_
#define _cSkinnedMeshComponent_HG_

#include <string>
#include <vector>

#include <glm/vec3.hpp>

#include "Global.h"
#include "cComponent.h"
#include "cSimpleAssimpSkinnedMeshLoader_OneMesh.h"
#include "cAnimationState.h"

class cSkinnedMeshComponent : public cComponent {
public:
	
	cSimpleAssimpSkinnedMesh* pSkinnedMesh;
	cAnimationState* pAniState;
	std::string meshName;

	// Extent Values for skinned mesh
	// These can be updated per frame, from the "update skinned mesh" call
	glm::vec3 minXYZ_from_SM_Bones;
	glm::vec3 maxXYZ_from_SM_Bones;

	// Store all the bones for this model, buing updated per frame
	std::vector< glm::mat4x4 > vecObjectBoneTransformation;


	cSkinnedMeshComponent();
	virtual ~cSkinnedMeshComponent(void){}

	bool assimpToVAOConverter();

	cMesh* pMesh;
};


#endif
