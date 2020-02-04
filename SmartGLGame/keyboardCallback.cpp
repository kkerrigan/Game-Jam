#include <Global.h>
#include <cSceneManager.h>
#include <cEntityBuilder.h>
#include <cRenderMeshComponent.h>
#include "cParticleComponent.h"
#include "cPhysicsComponent.h"
#include "cAnimationState.h"
#include "cSkinnedMeshComponent.h"
#include "cPathFinding.h"
#include "cMeshRenderSystem.h"

bool isShiftDown(GLFWwindow* window);
bool isCtrlDown(GLFWwindow* window);
bool isAltDown(GLFWwindow* window);
bool areAllModifiersUp(GLFWwindow* window);

extern float camera_target_y;

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	cSceneManager* pSceneManager = cSceneManager::getInstance();
	cScene* pScene = pSceneManager->getActiveScene();
	sLight* selectedLight = pScene->getSelectedLight();
	cEntity* selectedEntity = pScene->getSelectedEntity();

	cEntity* the_skinned_mesh = cSceneManager::getInstance()->getActiveScene()->getSelectedEntity();
	cSkinnedMeshComponent* skinned_mesh_component = static_cast<cSkinnedMeshComponent*>(the_skinned_mesh->components[10]);
	cRenderMeshComponent* render_mesh_component = static_cast<cRenderMeshComponent*>(the_skinned_mesh->components[3]);

	cSkinnedMeshComponent* animation = static_cast<cSkinnedMeshComponent*>(the_skinned_mesh->components[10]);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (animation->pAniState->vecAnimationQueue.empty() || animation->pAniState->vecAnimationQueue.front().name != "Jump Run")
		{
			cAnimationState::sStateDetails nextAnimation;
			nextAnimation.name = "Jump Run";
			nextAnimation.totalTime = skinned_mesh_component->pSkinnedMesh->getDuration(nextAnimation.name);
			nextAnimation.frameStepTime = 0.01f;
			animation->pAniState->vecAnimationQueue.push(nextAnimation);
		}

	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{

		if (animation->pAniState->vecAnimationQueue.empty() || animation->pAniState->vecAnimationQueue.front().name != "Jump Run")
		{
			cAnimationState::sStateDetails nextAnimation;
			nextAnimation.name = "Jump";
			nextAnimation.totalTime = skinned_mesh_component->pSkinnedMesh->getDuration(nextAnimation.name);
			nextAnimation.frameStepTime = 0.01f;
			animation->pAniState->vecAnimationQueue.push(nextAnimation);
		}
	}


	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}


	if (key == GLFW_KEY_F2 && action == GLFW_PRESS) {
		pScene->show_camera_debug = !pScene->show_camera_debug;
	}

	if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
	{
		pScene->cycle_entities(false);
	}

	if (areAllModifiersUp(window)) {
		
		if(key==GLFW_KEY_C && action == GLFW_PRESS)
		{
			cSceneManager::getInstance()->getActiveScene()->follow_cam_enabled = !cSceneManager::getInstance()->getActiveScene()->follow_cam_enabled;
		}
		
		if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) {
			cAnimationState::sStateDetails nextAnimation;
			nextAnimation.name = "Dance";
			nextAnimation.totalTime = skinned_mesh_component->pSkinnedMesh->getDuration(nextAnimation.name);
			nextAnimation.frameStepTime = 0.01f;
			animation->pAniState->vecAnimationQueue.push(nextAnimation);
		}

		if (key == GLFW_KEY_B && action == GLFW_PRESS) {
			cMeshRenderSystem::getInstance()->toggle_blur();
		}
	}



	if (isShiftDown(window)) {

	}

	if (isAltDown(window)) {

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
			cRenderMeshComponent* meshComonent = (cRenderMeshComponent*)selectedEntity->components[3];

			meshComonent->bIsWireFrame = !meshComonent->bIsWireFrame;
		}
	}

	if (isCtrlDown(window)) {
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			pSceneManager->getActiveScene()->serialize();
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { // Toggle all AABBs for debug
			pScene->bIsRenderDebug = !pScene->bIsRenderDebug;
		}
	}
}