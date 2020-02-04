#ifndef _cSimpleAssimpSkinnedMesh_HG
#define _cSimpleAssimpSkinnedMesh_HG

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>
#include <vector>
#include <array>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

#include "sVertex_xyz_rgba_n_uv2_bt_4Bones.h"

// Used by the game object and the rendering
//#include "../cMesh.h"			// For loading into the VAO manager
//#include "cMeshObject.h"
#include "cMesh.h"
#include <Windows.h>

//#include "../sMeshDrawInfo.h"	// For the draw call
//Inspired by this page: http://ogldev.atspace.co.uk/www/tutorial38/tutorial38.html


class cSimpleAssimpSkinnedMesh {
public:
	static const int MAX_BONES_PER_VERTEX = 4;
private:

	struct sVertexBoneData {
		
		std::array<float, MAX_BONES_PER_VERTEX> ids;
		std::array<float, MAX_BONES_PER_VERTEX> weights;

		void addBoneData(unsigned int BoneID, float Weight);
	};

	struct sBoneInfo {
		
		glm::mat4 BoneOffset;
		glm::mat4 FinalTransformation;
		glm::mat4 ObjectBoneTransformation;
	};

public:
	// Mesh and VAO and VBO infor
	//unsigned int m_VBO_ID;				// Vertex buffer object 
	unsigned int m_numberOfVertices;
	//unsigned int m_indexBuf_ID;			// Index buffer referring to VBO
	unsigned int m_numberOfIndices;
	unsigned int m_numberOfTriangles;
	//bool m_bVBO_created;
	//// Remember: a VAO is a VBO+shaderProgram combination (i.e. you need 1 VAO ++PER++ VBO+shaderID)
	//// (this is only an issue if you are using mulitple shaders; if you are using a single "uber" shader, you're fine)
	//unsigned int m_VAO_ID;				// Vertex Array Object
	//unsigned int m_shaderIDMatchingVAO;	// The shader prod ID matching this VBO
	//bool m_bVAO_created;
//	// Actual vertex data
//	vert_XYZ_RGBA_N_STU_TanBi_4Bones* m_pVertexData;

	std::string fileName;
	std::string friendlyName;

	const aiScene* pScene;		// "pretty" mesh we update and draw
	Assimp::Importer mImporter;

	//std::map< std::string /*animationfile*/,
	//	const aiScene* > mapAnimationNameTo_pScene;		// Animations

	cSimpleAssimpSkinnedMesh(void);
	virtual ~cSimpleAssimpSkinnedMesh(void);

	bool loadMeshFromFile(const std::string &filename);		// mesh we draw
	bool loadMeshAnimation(const std::string &friendlyName, const std::string &filename);	// Only want animations
	bool loadMeshAnimation_Threaded(const std::string &friendlyName, const std::string &filename);	

	struct sAnimationInfo
	{
		std::string friendlyName;
		std::string fileName;
		const aiScene* pAIScene;
	};

	CRITICAL_SECTION animation_map_lock;
	void add_new_animation_to_map(sAnimationInfo info);
	std::map< std::string /*animation FRIENDLY name*/, sAnimationInfo > mapAnimationFriendlyNameTo_pScene;

	// Looks in the animation map and returns the total time
	float findAnimationTotalTime(std::string animationName);
	float findAnimationTicks(std::string animationName);

	// Creates a VBO, loads the current mesh, then creates a VAO for the current VBO+shader
	//bool CreateVBOfromCurrentMesh(void);
	//bool CreateVBOandVOAfromCurrentMesh( int shaderProgID, std::string &error );

	// Returns NULL (0) if there is no mesh at that index
	cMesh* createMeshObjectFromCurrentModel(unsigned int meshIndex = 0);

	// The sMeshDrawInfo is used during the render call to lookup
	//	VAO infomation. Also, the gross orientation of the model is from 
	//	here. So this meshDrawInfo is the mesh bind pose, really.
	//	bool CreateMeshDrawInfoFromCurrentModel( sMeshDrawInfo &meshDrawInfo );

//	void BoneTransform(float TimeInSeconds, 
//					   std::vector<glm::mat4> &FinalTransformation, 
//					   std::vector<glm::mat4> &Globals, 
//					   std::vector<glm::mat4> &Offsets);
	void boneTransform(float TimeInSeconds,
		std::string animationName,		// Now we can pick the animation
		std::vector<glm::mat4> &FinalTransformation,
		std::vector<glm::mat4> &Globals,
		std::vector<glm::mat4> &Offsets);

	unsigned int getNums(void) const { return this->mNumBones; }
	float getDuration(std::string animationName);

	std::vector<sVertexBoneData> vecVertexBoneData;	//Bones;
	glm::mat4 mGlobalInverseTransformation;


	bool initialize(void);

	void calcInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, aiQuaternion& out);
	void calcInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, aiVector3D& out);
	void calcInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, aiVector3D& out);

	void calcGLMInterpolatedRotation(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::quat& out);
	void calcGLMInterpolatedPosition(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3& out);
	void calcGLMInterpolatedScaling(float AnimationTime, const aiNodeAnim* pNodeAnim, glm::vec3& out);

	unsigned int findRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int findScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);

	const aiNodeAnim* findNodeAnimationChannel(const aiAnimation* pAnimation, aiString nodeOrBoneName);

	//void ReadNodeHeirarchy(float AnimationTime, 
	//					   const aiNode* pNode, 
	//					   const glm::mat4 &parentTransformMatrix);

	void readNodeHeirarchy(float AnimationTime,
		std::string animationName,		// Now with more "choose animation"
		const aiNode* pNode,
		const glm::mat4 &parentTransformMatrix);

	void loadBones(const aiMesh* Mesh, std::vector<sVertexBoneData> &Bones);

	void shutErDown(void);

	//	std::vector<sMeshEntry> mMeshEntries;
	std::map<std::string /*BoneName*/, unsigned int /*BoneIndex*/> m_mapBoneNameToBoneIndex;	//mMapping;
	std::vector<sBoneInfo> mBoneInfo;
	unsigned int mNumBones;	//mNums;


	void GetListOfBoneIDandNames( std::vector<std::string> &vecBoneNames , const std::string name);
};

#endif
