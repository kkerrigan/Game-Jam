#ifndef _cMeshRenderSystem_HG_
#define _cMeshRenderSystem_HG_

#include <vector>
#include <glm/mat4x4.hpp>

#include "iSystem.h"
#include "cRenderMeshComponent.h"
#include "cBatchRenderSystem.h"
#include "cFBO.h"
#include "cShadowFBO.h"

class cMeshRenderSystem : public iSystem {
public:

	// Shader Uniforms
	GLint matModel_UniLoc;
	GLint matView_UniLoc;
	GLint matProj_UniLoc;
	GLint matModelInvTrans_UniLoc;
	GLint objectEyePos_UniLoc;
	GLint objectColour_UniLoc;
	GLint objectSpecular_UniLoc;
	GLint useVertexColour_UniLoc;
	GLint bDontUseLighting_UniLoc;
	GLint colorByHeight_UniLoc;
	GLint wholeObjectAlphaTrans_UniLoc;
	GLint skyBoxCubeMap_UniLoc;
	GLint useSkyBoxTexture_UniLoc;
	GLint render_target_overlay_UniLoc;
	bool bShaderUniformsLoaded;
	GLint is_h_bloom_loc;


	GLint bIsAskinnedMesh_UniLoc;
	GLint numBonesUsed_UniLoc;
	GLint bones_UniLoc;

	GLuint skyBoxTextureUnitID;

	// Texture uniforms
	GLint tex00_UniLoc;
	GLint tex01_UniLoc;
	GLint tex02_UniLoc;
	GLint tex03_UniLoc;
	GLint tex04_UniLoc;
	GLint tex05_UniLoc;
	GLint tex06_UniLoc;
	GLint tex07_UniLoc;
	GLint texBW_0_UniLoc;
	GLint texBW_1_UniLoc;
	GLint renderPassNumber_UniLoc;
	GLint texPass1OutputTexture_UniLoc;
	GLint bSecondPass_UniLoc;
	bool bTextureUniformsLoaded;

	bool blur_enabled = false;

	unsigned int tv_num_of_frames_before_update = 60;
	unsigned int elapsed_frames = 0;

	cBatchRenderSystem batch_render;

	// Singleton setup
	static cMeshRenderSystem* getInstance();
	cMeshRenderSystem(cMeshRenderSystem const&) = delete;
	void operator=(cMeshRenderSystem const&) = delete;

	void loadShaderUniforms(GLint shaderProgramID);
	void update(double deltaTime);
	void registerEntity(cEntity* entity);
	void unregisterEntity(cEntity* entity);
	void loadTextureUniforms(GLint shaderProgramID);
	void bindTextures(cEntity* pCurrentEntity, GLuint shaderProgramID, int pass);
	void drawSkyBox(GLint shader_id);
	void drawObject(cEntity* pCurrentMesh, glm::mat4x4 &matModel, int shaderProgramID, int pass, float dt);
	void drawScene_Simple(std::vector<cEntity*> currentEntities, GLuint shaderProgramID, unsigned int passNumber, float dt);

	void draw_instanced_objects(cEntity* entity, int shader_program);
	void toggle_blur();

private:
	std::vector<cEntity*> entitiesToUpdate;

	cMeshRenderSystem();
	~cMeshRenderSystem();

	void shadow_update();
	void post_process_blur();

	cFBO* shadow_fbo;
};

#endif
