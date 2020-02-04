#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 

#include <algorithm>

#include "cEngine.h"
#include "cMeshRenderSystem.h"
#include "cRenderMeshComponent.h"
#include "cTransformComponent.h"
#include "cVAOMeshManager.h"
#include "sLight.h"
#include "cShaderManager.h"
#include "cSceneManager.h"
#include "cDebugRenderer.h"
#include "cBasicTextureManager.h"
#include "cPhysicsComponent.h"
#include "cFrameBufferComponent.h"
#include "cSkinnedMeshComponent.h"
#include "cInstancedMeshComponent.h"
#include "cBatchRenderSystem.h"
#include "cParticleSystem.h"

extern cEntity* pCube;
extern cEntity* pSphere;

class cPredEntityIsCloser {
public:
	cPredEntityIsCloser(glm::vec3 cameraEye_) {
		this->cameraEye = cameraEye_;
		return;
	}
	glm::vec3 cameraEye;

	bool operator() (const cEntity* P1, const cEntity* P2) {

		cTransformComponent* entityOneTrans = static_cast<cTransformComponent*>(P1->components[1]);
		cTransformComponent* entityTwoTrans = static_cast<cTransformComponent*>(P2->components[1]);

		float P1_dist_to_camera = glm::distance(entityOneTrans->get_position(), this->cameraEye);
		float P2_dist_to_camera = glm::distance(entityTwoTrans->get_position(), this->cameraEye);

		return P1_dist_to_camera > P2_dist_to_camera;
	}
};

cMeshRenderSystem::cMeshRenderSystem() {

	// Shader uniforms
	this->matModel_UniLoc = -1;
	this->matView_UniLoc = -1;
	this->matProj_UniLoc = -1;
	this->matModelInvTrans_UniLoc = -1;
	this->objectEyePos_UniLoc = -1;
	this->objectColour_UniLoc = -1;
	this->objectSpecular_UniLoc = -1;
	this->useVertexColour_UniLoc = -1;
	this->bDontUseLighting_UniLoc = -1;
	this->colorByHeight_UniLoc = -1;
	this->wholeObjectAlphaTrans_UniLoc = -1;
	this->skyBoxCubeMap_UniLoc = -1;
	this->useSkyBoxTexture_UniLoc = -1;
	this->bShaderUniformsLoaded = false;

	this->skyBoxTextureUnitID = 30;

	// Skinned Mesh uniforms
	this->bIsAskinnedMesh_UniLoc = -1;
	this->numBonesUsed_UniLoc = -1;
	this->bones_UniLoc = -1;

	this->tex00_UniLoc = -1;
	this->tex01_UniLoc = -1;
	this->tex02_UniLoc = -1;
	this->tex03_UniLoc = -1;
	this->tex04_UniLoc = -1;
	this->tex05_UniLoc = -1;
	this->tex06_UniLoc = -1;
	this->tex07_UniLoc = -1;
	this->texBW_0_UniLoc = -1;
	this->texBW_1_UniLoc = -1;
	this->renderPassNumber_UniLoc = -1;
	this->bSecondPass_UniLoc = -1;
	this->bTextureUniformsLoaded = false;

	return;
}

cMeshRenderSystem::~cMeshRenderSystem() {}

void cMeshRenderSystem::shadow_update()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->shadow_fbo->ID);

	// Projection
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 3000.0f;
	lightProjection = glm::ortho(-256.0f, 256.0f, -256.0f, 256.0f, near_plane, far_plane);

	//float timeOfDay = g_environment.getTimeOfDay();
	//if (timeOfDay >= 6.0f && timeOfDay <= 18.0f)
   // {
	lightView = glm::lookAt(glm::vec3(0.0f, 1000.0, 0.0f), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	// }
	// else
	// {
	//     lightView = glm::lookAt(g_environment.getMoonPosition() * 0.25f, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	// }

	lightSpaceMatrix = lightProjection * lightView;
	//glUniformMatrix4fv(this->matView_UniLoc, 1, GL_FALSE, &lightView[0][0]);
	//glUniformMatrix4fv(this->matProj_UniLoc, 1, GL_FALSE, &lightProjection[0][0]);
	//glUniformMatrix4fv(glGetUniformLocation(cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader"), "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
	//glUniform3f(this->objectEyePos_UniLoc, 0.0,1000.0,0);

	this->shadow_fbo->clearBuffers(true, true);

	this->drawScene_Simple(this->entitiesToUpdate, cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader"), 1, 0.0f);

}

void cMeshRenderSystem::post_process_blur()
{

	//cSceneManager* sceneManager = cSceneManager::getInstance();
	//cScene* activeScene = sceneManager->getActiveScene();

	//// Switch to the shader we want

	//GLint program = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
	//cShaderManager::getInstance()->useShaderProgram(program);

	//float ratio;
	//int width, height;
	//glm::mat4x4 matProjection = glm::mat4(1.0f);
	//glm::mat4x4	matView = glm::mat4(1.0f);

	//cFBO* blur_h_fbo = static_cast<cFrameBufferComponent*>(blur_h->components[9])->pFbo;
	//glBindFramebuffer(GL_FRAMEBUFFER, blur_h_fbo->ID);
	//blur_h_fbo->clearBuffers(true, true);
	//glfwGetFramebufferSize(cEngine::getInstance()->getWindow(), &width, &height);

	//{
	//	ratio = width / (float)height;
	//	glViewport(0, 0, width, height);

	//	glEnable(GL_DEPTH);			 // Enables the KEEPING of the depth information
	//	glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
	//	glEnable(GL_CULL_FACE);		// Discared "back facing" triangles

	//	matProjection = glm::perspective(0.6f,
	//		ratio,			// Aspect ratio
	//		0.1f,			// Near clipping plane
	//		10000.0f);		// Far clipping plane

	//	matView = glm::lookAt(
	//		glm::vec3(117.18, 12.43, 27.86),
	//		glm::vec3(116.17, 12.11, 27.58),
	//		glm::vec3(-0.32, 0.95, -0.06)
	//	);

	//	glUniformMatrix4fv(this->matView_UniLoc, 1, GL_FALSE, glm::value_ptr(matView));
	//	glUniformMatrix4fv(this->matProj_UniLoc, 1, GL_FALSE, glm::value_ptr(matProjection));
	//	glUniform1f(this->is_h_bloom_loc, GL_TRUE);
	//	glm::mat4x4 matModel(1.0f);
	//	cTransformComponent* quad_trans = static_cast<cTransformComponent*>(quad->components[1]);
	//		quad_trans->scale.x = 1.0f;
	//		quad_trans->scale.y = 1.0f;
	//		quad_trans->scale.z = ratio;

	//	this->drawObject(quad, matModel, program, 3, 0.0);
	//	glUniform1f(this->is_h_bloom_loc, GL_FALSE);
	//}
}

cMeshRenderSystem* cMeshRenderSystem::getInstance() {
	static cMeshRenderSystem instance;

	return &instance;
}


cEntity* quad = nullptr; //final output fbo
cEntity* tv1 = nullptr;
cEntity* tv2 = nullptr;

void cMeshRenderSystem::update(double deltaTime) {
	++elapsed_frames;
	cSceneManager* sceneManager = cSceneManager::getInstance();
	cScene* activeScene = sceneManager->getActiveScene();

	//day/night cycle/waves
	activeScene->update(deltaTime);

	GLint program = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
	cShaderManager::getInstance()->useShaderProgram(program);

	// 1 = 1st pass (the actual scene)
	// 2 = 2nd pass (rendering what we drew to the output
	if (quad == nullptr) //if the quad is not loaded all the other pointers are not loaded
	{
		quad = cSceneManager::getInstance()->getActiveScene()->findObjectByFriendlyName("quad");
		tv1 = cSceneManager::getInstance()->getActiveScene()->findObjectByFriendlyName("tv1");
		tv2 = cSceneManager::getInstance()->getActiveScene()->findObjectByFriendlyName("tv2");
	}

	float ratio;
	int width, height;
	glm::mat4x4 matProjection = glm::mat4(1.0f);
	glm::mat4x4	matView = glm::mat4(1.0f);

	glUniform3f(this->objectEyePos_UniLoc, -145.0, 16.53, 29.58);

	//in range of the tvs? (only update one per frame as the draw is expensive)
	if (glm::distance(static_cast<cTransformComponent*>(tv1->components[1])->get_position(), activeScene->flyCamera.eye) < 10.0f)
	{
		//and we have passed enough time to redraw the tv
		if (elapsed_frames > tv_num_of_frames_before_update)
		{
			if (elapsed_frames % 2 == 1)
			{
				//draw the tvs
				cFBO* fbo_tv_1 = static_cast<cFrameBufferComponent*>(tv1->components[9])->pFbo;
				glBindFramebuffer(GL_FRAMEBUFFER, fbo_tv_1->ID);
				fbo_tv_1->clearBuffers(true, true);
				glfwGetFramebufferSize(cEngine::getInstance()->getWindow(), &width, &height);

				{
					ratio = width / (float)height;
					glViewport(0, 0, width, height);

					glEnable(GL_DEPTH);			 // Enables the KEEPING of the depth information
					glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
					glEnable(GL_CULL_FACE);		// Discared "back facing" triangles

					matProjection = glm::perspective(0.6f,
						ratio,			// Aspect ratio
						0.1f,			// Near clipping plane
						10000.0f);		// Far clipping plane

					matView = glm::lookAt(glm::vec3(-145.0, 16.53, 29.58), glm::vec3(-144.91, 16.23, 28.57), glm::vec3(-0.06, 0.96, -0.30));

					// Load the shader uniform locations if not already done
					if (!this->bShaderUniformsLoaded) {
						this->loadShaderUniforms(program);
					}

					glUniformMatrix4fv(this->matView_UniLoc, 1, GL_FALSE, glm::value_ptr(matView));
					glUniformMatrix4fv(this->matProj_UniLoc, 1, GL_FALSE, glm::value_ptr(matProjection));

					// Draw Sky box first
					this->drawSkyBox(program);
					this->drawScene_Simple(this->entitiesToUpdate, program, 1, deltaTime);
					cParticleSystem::getInstance()->update(0.0f);
				}
			}
			else
			{
				cFBO* fbo_tv_2 = static_cast<cFrameBufferComponent*>(tv2->components[9])->pFbo;
				glBindFramebuffer(GL_FRAMEBUFFER, fbo_tv_2->ID);
				fbo_tv_2->clearBuffers(true, true);
				glfwGetFramebufferSize(cEngine::getInstance()->getWindow(), &width, &height);

				{
					ratio = width / (float)height;
					glViewport(0, 0, width, height);

					glEnable(GL_DEPTH);			 // Enables the KEEPING of the depth information
					glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
					glEnable(GL_CULL_FACE);		// Discared "back facing" triangles

					matProjection = glm::perspective(0.6f,
						ratio,			// Aspect ratio
						0.1f,			// Near clipping plane
						10000.0f);		// Far clipping plane

					matView = glm::lookAt(glm::vec3(131.49, 28.23, 108.91), glm::vec3(130.66, 27.89, 108.46), glm::vec3(-0.3, 0.94, -0.15));

					// Load the shader uniform locations if not already done
					if (!this->bShaderUniformsLoaded) {
						this->loadShaderUniforms(program);
					}

					glUniformMatrix4fv(this->matView_UniLoc, 1, GL_FALSE, glm::value_ptr(matView));
					glUniformMatrix4fv(this->matProj_UniLoc, 1, GL_FALSE, glm::value_ptr(matProjection));

					// Draw Sky box first
					this->drawSkyBox(program);
					this->drawScene_Simple(this->entitiesToUpdate, program, 1, deltaTime);
					cParticleSystem::getInstance()->update(0.0f);

					elapsed_frames = 0;
				}
			}
		}




	}



	//========================================================================================

	cFBO* fbo = static_cast<cFrameBufferComponent*>(quad->components[9])->pFbo;
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID);
	fbo->clearBuffers(true, true);

	glfwGetFramebufferSize(cEngine::getInstance()->getWindow(), &width, &height);

	ratio = width / (float)height;
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
	glEnable(GL_CULL_FACE);		// Discared "back facing" triangles

	glUniform3f(this->objectEyePos_UniLoc, activeScene->flyCamera.eye.x, activeScene->flyCamera.eye.y, activeScene->flyCamera.eye.z);

	matProjection = glm::perspective(0.6f,
		ratio,			// Aspect ratio
		0.1f,			// Near clipping plane
		10000.0f);		// Far clipping plane

	matView = glm::lookAt(activeScene->flyCamera.eye, activeScene->flyCamera.getAtInWorldSpace(), activeScene->flyCamera.getUpVector());

	cTransformComponent* quad_trans = static_cast<cTransformComponent*>(quad->components[1]);
	quad_trans->scale.x = 1.0f;
	quad_trans->scale.y = 1.0f;
	quad_trans->scale.z = ratio;

	// Load the shader uniform locations if not already done
	if (!this->bShaderUniformsLoaded) {
		this->loadShaderUniforms(program);
	}

	glUniformMatrix4fv(this->matView_UniLoc, 1, GL_FALSE, glm::value_ptr(matView));
	glUniformMatrix4fv(this->matProj_UniLoc, 1, GL_FALSE, glm::value_ptr(matProjection));

	// Draw Sky box first
	this->drawSkyBox(program);
	this->drawScene_Simple(this->entitiesToUpdate, program, 1, deltaTime);
	//cDebugRenderer::getInstance()->RenderDebugObjects(matView, matProjection, deltaTime);

	glm::mat4 matModelTv1 = glm::mat4(1.0f);
	glm::mat4 matModelTv2 = glm::mat4(1.0f);
	this->drawObject(tv1, matModelTv1, program, 2, 0.0f);
	this->drawObject(tv2, matModelTv2, program, 2, 0.0f);

	// if the update callback is set call it
	if (cEngine::getInstance()->pFuncUpdateCallback != nullptr) {
		cEngine::getInstance()->pFuncUpdateCallback(deltaTime);
	}

	cParticleSystem::getInstance()->update(deltaTime);

	//SECOND PASS (this is the actual output from the offscreen texture)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);		// Points to the "regular" frame buffer

	glfwGetFramebufferSize(cEngine::getInstance()->getWindow(), &width, &height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cTransformComponent* quad_trans2 = static_cast<cTransformComponent*>(quad->components[1]);
	quad_trans2->scale.x = 1.0f;
	quad_trans2->scale.y = 1.0f;
	quad_trans2->scale.z = ratio;

	//glEnable(GL_DEPTH);		// Enables the KEEPING of the depth information
	glDisable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
	glDisable(GL_CULL_FACE);	// Discared "back facing" triangles

	// 4. Draw a single quad		
	glm::mat4x4 matModel = glm::mat4(1.0f);	// identity
	if (this->blur_enabled) //to blur or not to blur
	{
		this->drawObject(quad, matModel, program, 3, deltaTime);
	}
	else
	{
		this->drawObject(quad, matModel, program, 2, deltaTime);
	}
}

void cMeshRenderSystem::drawSkyBox(GLint shader_id) {
	cScene* activeScene = cSceneManager::getInstance()->getActiveScene();
	cEntity* pSkyBox = activeScene->findObjectByFriendlyName("SkyBoxObject");
	cTransformComponent* pSkyboxTransComp = static_cast<cTransformComponent*>(pSkyBox->components[1]);
	cRenderMeshComponent* pSkyboxMeshComp = static_cast<cRenderMeshComponent*>(pSkyBox->components[3]);

	// Draw the skybox first 
	pSkyboxTransComp->set_position(activeScene->flyCamera.eye);
	pSkyboxMeshComp->bIsVisible = true;
	pSkyboxMeshComp->bIsWireFrame = false;

	// Bind the cube map textures to the cube maps in the shader
	glActiveTexture(30 + GL_TEXTURE0);
	int cubeMapTextureID = cBasicTextureManager::getInstance()->getTextureIDFromName("night_cube_map");
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
	glUniform1i(glGetUniformLocation(shader_id, "night_skybox_texture"), 30);

	glActiveTexture(31 + GL_TEXTURE0);
	cubeMapTextureID = cBasicTextureManager::getInstance()->getTextureIDFromName("dawn_cube_map");
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
	glUniform1i(glGetUniformLocation(shader_id, "dawn_skybox_texture"), 31);

	glActiveTexture(32 + GL_TEXTURE0);
	cubeMapTextureID = cBasicTextureManager::getInstance()->getTextureIDFromName("morning_cube_map");
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
	glUniform1i(glGetUniformLocation(shader_id, "morning_skybox_texture"), 32);

	glActiveTexture(33 + GL_TEXTURE0);
	cubeMapTextureID = cBasicTextureManager::getInstance()->getTextureIDFromName("day_cube_map");
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
	glUniform1i(glGetUniformLocation(shader_id, "day_skybox_texture"), 33);

	glActiveTexture(34 + GL_TEXTURE0);
	cubeMapTextureID = cBasicTextureManager::getInstance()->getTextureIDFromName("dusk_cube_map");
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
	glUniform1i(glGetUniformLocation(shader_id, "dusk_skybox_texture"), 34);

	//weights
	glUniform4f(glGetUniformLocation(shader_id, "skybox_weights[0]"), activeScene->skybox_weights[0].x, activeScene->skybox_weights[0].y, activeScene->skybox_weights[0].z, activeScene->skybox_weights[0].a);
	glUniform4f(glGetUniformLocation(shader_id, "skybox_weights[1]"), activeScene->skybox_weights[1].x, 0.0f, 0.0f, 0.0f);

	glUniform1f(this->useSkyBoxTexture_UniLoc, (float)GL_TRUE);

	glm::mat4 matIdentity = glm::mat4(1.0f);
	GLint program = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");

	this->drawObject(pSkyBox, matIdentity, program, 1, 0.0f);

	pSkyboxMeshComp->bIsVisible = false;
	glUniform1f(this->useSkyBoxTexture_UniLoc, (float)GL_FALSE);
}

void cMeshRenderSystem::loadShaderUniforms(GLint shaderProgramID) {

	this->matModel_UniLoc = glGetUniformLocation(shaderProgramID, "matModel");
	this->matView_UniLoc = glGetUniformLocation(shaderProgramID, "matView");
	this->matProj_UniLoc = glGetUniformLocation(shaderProgramID, "matProj");
	this->matModelInvTrans_UniLoc = glGetUniformLocation(shaderProgramID, "matModelInvTrans");
	this->objectEyePos_UniLoc = glGetUniformLocation(shaderProgramID, "eyeLocation");
	this->objectColour_UniLoc = glGetUniformLocation(shaderProgramID, "objectDiffuse");
	this->objectSpecular_UniLoc = glGetUniformLocation(shaderProgramID, "objectSpecular");
	this->useVertexColour_UniLoc = glGetUniformLocation(shaderProgramID, "useVertexColour");
	this->bDontUseLighting_UniLoc = glGetUniformLocation(shaderProgramID, "bDontUseLighting");
	this->colorByHeight_UniLoc = glGetUniformLocation(shaderProgramID, "colorBasedOnHeight");
	this->wholeObjectAlphaTrans_UniLoc = glGetUniformLocation(shaderProgramID, "wholeObjectAlphaTransparency");
	this->skyBoxCubeMap_UniLoc = glGetUniformLocation(shaderProgramID, "textureSkyBox");
	this->useSkyBoxTexture_UniLoc = glGetUniformLocation(shaderProgramID, "useSkyBoxTexture");

	this->bIsAskinnedMesh_UniLoc = glGetUniformLocation(shaderProgramID, "bIsASkinnedMesh");
	this->numBonesUsed_UniLoc = glGetUniformLocation(shaderProgramID, "numBonesUsed");
	this->bones_UniLoc = glGetUniformLocation(shaderProgramID, "bones");
	this->is_h_bloom_loc = glGetUniformLocation(shaderProgramID, "is_h_bloom");




	this->bShaderUniformsLoaded = true;
}

void cMeshRenderSystem::loadTextureUniforms(GLint shaderProgramID) {

	this->tex00_UniLoc = glGetUniformLocation(shaderProgramID, "texture00");		// uniform sampler2D texture00;
	this->tex01_UniLoc = glGetUniformLocation(shaderProgramID, "texture01");		// uniform sampler2D texture01;
	this->tex02_UniLoc = glGetUniformLocation(shaderProgramID, "texture02");		// uniform sampler2D texture02;
	this->tex03_UniLoc = glGetUniformLocation(shaderProgramID, "texture03");		// uniform sampler2D texture03;
	this->tex04_UniLoc = glGetUniformLocation(shaderProgramID, "texture04");		// uniform sampler2D texture04;
	this->tex05_UniLoc = glGetUniformLocation(shaderProgramID, "texture05");		// uniform sampler2D texture05;
	this->tex06_UniLoc = glGetUniformLocation(shaderProgramID, "texture06");		// uniform sampler2D texture06;
	this->tex07_UniLoc = glGetUniformLocation(shaderProgramID, "texture07");		// uniform sampler2D texture07;
	this->texBW_0_UniLoc = glGetUniformLocation(shaderProgramID, "texBlendWeights[0]");	// uniform vec4 texBlendWeights[2];
	this->texBW_1_UniLoc = glGetUniformLocation(shaderProgramID, "texBlendWeights[1]");	// uniform vec4 texBlendWeights[2];
	this->renderPassNumber_UniLoc = glGetUniformLocation(shaderProgramID, "renderPassNumber");	// uniform vec4 texBlendWeights[2];;
	this->texPass1OutputTexture_UniLoc = glGetUniformLocation(shaderProgramID, "texPass1OutputTexture");	// uniform vec4 texBlendWeights[2];;
	this->bSecondPass_UniLoc = glGetUniformLocation(shaderProgramID, "bSecondPass");
	this->render_target_overlay_UniLoc = glGetUniformLocation(shaderProgramID, "render_target_overlay");
	this->bTextureUniformsLoaded = true;

	return;
}

void cMeshRenderSystem::registerEntity(cEntity* entity) {
	this->entitiesToUpdate.push_back(entity);
}

void cMeshRenderSystem::unregisterEntity(cEntity* entity) {
	for (auto iter = this->entitiesToUpdate.begin(); iter != this->entitiesToUpdate.end(); ++iter)
	{
		if (*iter == entity)
		{
			this->entitiesToUpdate.erase(iter);
			return;
		}
	}
}

void cMeshRenderSystem::bindTextures(cEntity* pCurrentEntity, GLuint shaderProgramID, int pass) {

	// Load texture uniforms if they are not already set
	if (!this->bTextureUniformsLoaded) {
		this->loadTextureUniforms(shaderProgramID);
	}

	// ******************************************************************** 
	//    _  _              _ _       ___ ___  ___    _           _                  _    _         _ _           
	//   | || |__ _ _ _  __| | |___  | __| _ )/ _ \  | |_ _____ _| |_ _  _ _ _ ___  | |__(_)_ _  __| (_)_ _  __ _ 
	//   | __ / _` | ' \/ _` | / -_) | _|| _ \ (_) | |  _/ -_) \ /  _| || | '_/ -_) | '_ \ | ' \/ _` | | ' \/ _` |
	//   |_||_\__,_|_||_\__,_|_\___| |_| |___/\___/   \__\___/_\_\\__|\_,_|_| \___| |_.__/_|_||_\__,_|_|_||_\__, |
	//                                                                                                      |___/ 

	//Check to see if the current mesh has a cFrameBufferComponent.....
	// If it DOES, then we bind the texture to that, and exit.
	// We will be making this more sophisticated so that we can have
	cRenderMeshComponent* pCurrentMesh = static_cast<cRenderMeshComponent*>(pCurrentEntity->components[3]);
	cFrameBufferComponent* pCurrFBOComp = static_cast<cFrameBufferComponent*>(pCurrentEntity->components[9]);
	if ((pCurrentMesh->use_offscreen_texture && pCurrFBOComp != nullptr && (pass == 2 || pass == 3)) || cSceneManager::getInstance()->getActiveScene()->findObjectByFriendlyName("quad2") == pCurrentEntity) {

		// Connect the texture for this object to the FBO texture
		int currFboTextureUnit = pCurrFBOComp->getFboTextureUnit();
		glActiveTexture(GL_TEXTURE0 + currFboTextureUnit);

		// Connect the specific texture to THIS texture unit
		glBindTexture(GL_TEXTURE_2D, pCurrFBOComp->pFbo->colourTexture_0_ID);

		// Set the sampler (in the shader) to ALSO point to the texture unit
		// This one takes the unchanged texture unit numbers
		glUniform1i(this->texPass1OutputTexture_UniLoc, currFboTextureUnit);



		return;
	}

	if (pCurrentMesh->use_offscreen_texture) return;


	float blendWeights[8] = { 0 };

	for (int texBindIndex = 0; texBindIndex != pCurrentMesh->textures.size(); texBindIndex++) {

		// Bind to the the "texBindIndex" texture unit
		glActiveTexture(GL_TEXTURE0 + texBindIndex);

		// Connect the specific texture to THIS texture unit
		std::string texName = pCurrentMesh->textures[texBindIndex].name;

		GLuint texID = cBasicTextureManager::getInstance()->getTextureIDFromName(texName);

		glBindTexture(GL_TEXTURE_2D, texID);

		// Use a switch to pick the texture sampler and weight (strength)
		// BECAUSE the samplers can't be in an array
		switch (texBindIndex)
		{
		case 0:		// uniform sampler2D texture00  AND texBlendWeights[0].x;
			glUniform1i(this->tex00_UniLoc, texBindIndex);
			break;
		case 1:		// uniform sampler2D texture01  AND texBlendWeights[0].y;
			glUniform1i(this->tex01_UniLoc, texBindIndex);
			break;
		case 2:
			glUniform1i(this->tex02_UniLoc, texBindIndex);
			break;
		case 3:
			glUniform1i(this->tex03_UniLoc, texBindIndex);
			break;
		case 4:		// uniform sampler2D texture04  AND texBlendWeights[1].x;
			glUniform1i(this->tex04_UniLoc, texBindIndex);
			break;
		case 5:
			glUniform1i(this->tex05_UniLoc, texBindIndex);
			break;
		case 6:
			glUniform1i(this->tex06_UniLoc, texBindIndex);
			break;
		case 7:
			glUniform1i(this->tex07_UniLoc, texBindIndex);
			break;
		}//switch ( texBindIndex )

		// Set the blend weight (strengt)
		blendWeights[texBindIndex] = pCurrentMesh->textures[texBindIndex].strength;
	}

	// Set the weights (strengths) in the shader
	glUniform4f(this->texBW_0_UniLoc, blendWeights[0], blendWeights[1], blendWeights[2], blendWeights[3]);
	glUniform4f(this->texBW_1_UniLoc, blendWeights[4], blendWeights[5], blendWeights[6], blendWeights[7]);

	return;
}

float time_since = 0.f;
void cMeshRenderSystem::drawObject(cEntity* pEntity, glm::mat4x4 &matModel, int shaderProgramID, int pass, float dt) {

	cScene* activeScene = cSceneManager::getInstance()->getActiveScene();
	cTransformComponent* transformComponent = static_cast<cTransformComponent*>(pEntity->components[1]);
	cRenderMeshComponent* meshComponent = static_cast<cRenderMeshComponent*>(pEntity->components[3]);

	//new physics
	cPhysicsComponent* physics_component = static_cast<cPhysicsComponent*>(pEntity->components[8]);

	glUseProgram(shaderProgramID);

	// Is this object visible
	if (!meshComponent->bIsVisible) {
		return;
	}

	//is this an instanced mesh
	if (pEntity->components[12])
	{
		glUniform1f(glGetUniformLocation(shaderProgramID, "bUseWaterHeightMaps"), GL_FALSE);
		glUniform1f(glGetUniformLocation(shaderProgramID, "bUseSplatMap"), GL_FALSE);
		this->draw_instanced_objects(pEntity, shaderProgramID);
		return;
	}


	this->bindTextures(pEntity, shaderProgramID, pass);

	matModel = glm::mat4x4(1.0f);

	glm::vec3 position;
	glm::vec3 scale;
	glm::quat orientation;

	if (physics_component && physics_component->get_rigid_body() != nullptr)
	{
		physics::interfaces::iRigidBody* rigid_body = physics_component->get_rigid_body();
		position = rigid_body->get_position();
		scale = rigid_body->get_scale();
		orientation = rigid_body->get_orientation();

		//hack:if its the player we need to translate down a bit
		if (meshComponent->meshName == "RPG-Character")
		{
			position.y -= 0.45f;
		}
	}
	else
	{
		if (!transformComponent) return;
		position = transformComponent->get_position();
		scale = transformComponent->scale;
		orientation = transformComponent->getQOrientation();
	}

	glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f), position);
	matModel = matModel * matTranslation;

	glm::mat4 matQrotation = glm::mat4(orientation);
	matModel *= matQrotation;


	// And now scale
	glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
		scale);
	matModel = matModel * matScale;

	// Calculate the inverse transpose before the scaling
	glm::mat4 matModelInvTrans = glm::inverse(glm::transpose(matModel));


	if (meshComponent->bUseSplatMap)
	{
		glUniform1f(glGetUniformLocation(shaderProgramID, "bUseSplatMap"), GL_TRUE);
	}
	else
	{
		glUniform1f(glGetUniformLocation(shaderProgramID, "bUseSplatMap"), GL_FALSE);
	}

	if (meshComponent->bUseWaterHeightMaps)
	{
		glUniform1f(glGetUniformLocation(shaderProgramID, "bUseWaterHeightMaps"), GL_TRUE);
		glUniform4f(glGetUniformLocation(shaderProgramID, "waterOffsets"), activeScene->wave_offset.x, activeScene->wave_offset.y, 0.0f, 0.0f);
	}
	else
	{
		glUniform1f(glGetUniformLocation(shaderProgramID, "bUseWaterHeightMaps"), GL_FALSE);
	}

	glUniform1f(this->renderPassNumber_UniLoc, pass);

	if ((pass == 3.0f || pass == 2.0f) && !meshComponent->is_in_world_fbo)
	{
		glm::vec3 cameraFullScreenQuad = glm::vec3(0.0f, 0.0f, -3.0f);

		glUniform3f(this->objectEyePos_UniLoc, cameraFullScreenQuad.x, cameraFullScreenQuad.y, cameraFullScreenQuad.z);
		glm::mat4 matView = glm::lookAt(cameraFullScreenQuad,	// Eye
			glm::vec3(0.0f, 0.0f, 0.0f),		// At
			glm::vec3(0.0f, 1.0f, 0.0f));// Up

		glUniformMatrix4fv(this->matView_UniLoc, 1, GL_FALSE, glm::value_ptr(matView));
	}

	glUniformMatrix4fv(this->matModel_UniLoc, 1, GL_FALSE, glm::value_ptr(matModel));
	glUniformMatrix4fv(this->matModelInvTrans_UniLoc, 1, GL_FALSE, glm::value_ptr(matModelInvTrans));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Take the 4th value from the material and pass it as alpha
	glUniform1f(this->wholeObjectAlphaTrans_UniLoc, meshComponent->objColour.a);

	glUniform4f(this->objectColour_UniLoc,
		meshComponent->objColour.r,
		meshComponent->objColour.g,
		meshComponent->objColour.b,
		meshComponent->objColour.a);


	glUniform4f(this->objectSpecular_UniLoc, meshComponent->specular.r, meshComponent->specular.g, meshComponent->specular.b, meshComponent->specular.a);

	if (meshComponent->use_discard_texture)
	{
		glUniform1f(glGetUniformLocation(shaderProgramID, "use_discard_texture"), (float)GL_TRUE);
	}
	else
	{
		glUniform1f(glGetUniformLocation(shaderProgramID, "use_discard_texture"), (float)GL_FALSE);
	}

	if (meshComponent->bAddReflect)
	{
		glUniform1f(glGetUniformLocation(shaderProgramID, "bAddReflect"), (float)GL_TRUE);
	}
	else
	{
		glUniform1f(glGetUniformLocation(shaderProgramID, "bAddReflect"), (float)GL_FALSE);
	}

	if (meshComponent->bAddRefract)
	{
		glUniform1f(glGetUniformLocation(shaderProgramID, "bAddRefract"), (float)GL_TRUE);
	}
	else
	{
		glUniform1f(glGetUniformLocation(shaderProgramID, "bAddRefract"), (float)GL_FALSE);
	}

	if (meshComponent->bUseVertexColour)
	{
		glUniform1f(this->useVertexColour_UniLoc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(this->useVertexColour_UniLoc, (float)GL_FALSE);
	}

	if (meshComponent->colorByHeight) {
		glUniform1f(this->colorByHeight_UniLoc, (float)GL_TRUE);
	}
	else {
		glUniform1f(this->colorByHeight_UniLoc, (float)GL_FALSE);
	}

	if (meshComponent->bDontLight)
	{
		glUniform1f(this->bDontUseLighting_UniLoc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(this->bDontUseLighting_UniLoc, (float)GL_FALSE);
	}

	if (meshComponent->bIsWireFrame)
	{
		// Yes, draw it wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);	// Discared "back facing" triangles
		//glDisable( GL_DEPTH );		// Enables the KEEPING of the depth information
		//glDisable( GL_DEPTH_TEST );	// When drawing, checked the existing depth
	}
	else
	{
		// No, it's "solid" (or "Filled")
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);	// Discared "back facing" triangles
		//glEnable( GL_DEPTH );		// Enables the KEEPING of the depth information
		//glEnable( GL_DEPTH_TEST );	// When drawing, checked the existing depth
	}


	sModelDrawInfo modelInfo;

	cSkinnedMeshComponent* skinMeshComp = static_cast<cSkinnedMeshComponent*>(pEntity->components[10]);
	if (skinMeshComp == nullptr) {
		// Regular mesh
		modelInfo.meshFileName = meshComponent->meshName;
		glUniform1f(this->bIsAskinnedMesh_UniLoc, (float)GL_FALSE);
	}
	else {
		// Skinned mesh
		modelInfo.meshFileName = skinMeshComp->pSkinnedMesh->fileName;
		glUniform1f(this->bIsAskinnedMesh_UniLoc, (float)GL_TRUE);

		// Pass up the bone information
		std::vector<glm::mat4x4> vecFinalTransformation;
		std::vector<glm::mat4x4> vecOffsets;

		//Animation Midterm - Question # 4 (get_current_animation returns the front animation on the queue, if the queue is empty play idle)
		cAnimationState::sStateDetails& cur_animation = skinMeshComp->pAniState->get_current_animation();
		cur_animation.IncrementTime(dt);
		skinMeshComp->pSkinnedMesh->boneTransform(cur_animation.run_time, //curFrameTime
			cur_animation.name, // animation to play
			vecFinalTransformation, // Final bone transforms for mesh
			skinMeshComp->vecObjectBoneTransformation, // final location of bones
			vecOffsets); // local offset for each bone

		time_since += 0.01f;

		unsigned int numberOfBonesUsed = static_cast<unsigned int>(vecFinalTransformation.size());


		glm::mat4x4* pBoneMatrixArray = &(vecFinalTransformation[0]);

		glUniformMatrix4fv(this->bones_UniLoc, numberOfBonesUsed, GL_FALSE,
			(const GLfloat*)glm::value_ptr(*pBoneMatrixArray));


		for (unsigned int boneIndex = 0; boneIndex != numberOfBonesUsed; ++boneIndex) {

			glm::mat4 boneLocal = skinMeshComp->vecObjectBoneTransformation[boneIndex];

			// HACK: Need to add "uniform scale" to the mesh
			float scale = 1.0f;
			boneLocal = glm::scale(boneLocal, glm::vec3(scale,
				scale,
				scale));

			glm::vec4 boneBallLocation = boneLocal * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			boneBallLocation *= scale;

			// Update extents
			if (boneIndex == 0) {

				// For the 0th bone, just assume this is the extent
				skinMeshComp->minXYZ_from_SM_Bones = glm::vec3(boneBallLocation);
				skinMeshComp->maxXYZ_from_SM_Bones = glm::vec3(boneBallLocation);
			}
			else {
				// It's not the 0th bone, so compare with current max and min
				if (skinMeshComp->minXYZ_from_SM_Bones.x < boneBallLocation.x) { skinMeshComp->minXYZ_from_SM_Bones.x = boneBallLocation.x; }
				if (skinMeshComp->minXYZ_from_SM_Bones.y < boneBallLocation.y) { skinMeshComp->minXYZ_from_SM_Bones.y = boneBallLocation.y; }
				if (skinMeshComp->minXYZ_from_SM_Bones.z < boneBallLocation.z) { skinMeshComp->minXYZ_from_SM_Bones.z = boneBallLocation.z; }

				if (skinMeshComp->maxXYZ_from_SM_Bones.x > boneBallLocation.x) { skinMeshComp->maxXYZ_from_SM_Bones.x = boneBallLocation.x; }
				if (skinMeshComp->maxXYZ_from_SM_Bones.y > boneBallLocation.y) { skinMeshComp->maxXYZ_from_SM_Bones.y = boneBallLocation.y; }
				if (skinMeshComp->maxXYZ_from_SM_Bones.z > boneBallLocation.z) { skinMeshComp->maxXYZ_from_SM_Bones.z = boneBallLocation.z; }
			}


		} // for (unsigned int boneIndex = 0; 
	} // if (skinMeshComp == nullptr)
	// END OF: Skin mesh

	if (cVAOMeshManager::getInstance()->FindDrawInfoByModelName(modelInfo))
	{
		if (modelInfo.current_state == sModelDrawInfo::state::ready_for_gpu)
		{
			cVAOMeshManager::getInstance()->LoadModelIntoVAO(modelInfo, shaderProgramID);
		}

		if (modelInfo.current_state != sModelDrawInfo::state::loaded) return;

		glBindVertexArray(modelInfo.VAO_ID);

		glDrawElements(GL_TRIANGLES,
			modelInfo.numberOfIndices,
			GL_UNSIGNED_INT,
			nullptr);

		glBindVertexArray(0);

	}

	return;
}

void cMeshRenderSystem::drawScene_Simple(std::vector<cEntity*> currentEntities, GLuint shaderProgramID,
	unsigned passNumber, float dt)
{
	cScene* activeScene = cSceneManager::getInstance()->getActiveScene();

	std::vector<cEntity*> vec_transparentObj;

	for (unsigned int objIndex = 0; objIndex != currentEntities.size(); objIndex++) {
		glm::mat4x4 matModel = glm::mat4(1.0f);

		if (currentEntities[objIndex] == quad)
		{
			continue;
		}

		if (static_cast<cRenderMeshComponent*>(currentEntities[objIndex]->components[3])->objColour.a >= 1.0f) {
			this->drawObject(currentEntities[objIndex], matModel, shaderProgramID, passNumber, dt);
		}
		else {
			vec_transparentObj.push_back(currentEntities[objIndex]);
		}
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::sort(vec_transparentObj.begin(), vec_transparentObj.end(), cPredEntityIsCloser(activeScene->flyCamera.eye));
	for (unsigned int index = 0; index != vec_transparentObj.size(); ++index) {

		glm::mat4x4 matModel = glm::mat4(1.0f);
		this->drawObject(vec_transparentObj[index], matModel, shaderProgramID, passNumber, dt);
	}

	glDisable(GL_BLEND);
}

void cMeshRenderSystem::draw_instanced_objects(cEntity* entity, int shader_program)
{
	shader_program = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
	cScene* activeScene = cSceneManager::getInstance()->getActiveScene();

	this->bindTextures(entity, shader_program, 1);

	float ratio;
	int width, height;
	glm::mat4x4 matProjection = glm::mat4(1.0f);
	glm::mat4x4	matView = glm::mat4(1.0f);

	glfwGetFramebufferSize(cEngine::getInstance()->getWindow(), &width, &height);

	ratio = width / (float)height;


	for (const auto& x : static_cast<cInstancedMeshComponent*>(entity->components[12])->generate_batches())
	{
		auto test = batch_render.generate_vbo(x);
		batch_render.instance_draw_test(entity, test);
	}
}

void cMeshRenderSystem::toggle_blur()
{
	this->blur_enabled = !this->blur_enabled;
}
