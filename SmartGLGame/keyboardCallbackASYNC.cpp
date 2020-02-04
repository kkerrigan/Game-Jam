#include <Global.h>
#include <cSceneManager.h>
#include <cEntityBuilder.h>
#include <cTransformComponent.h>
#include <cRenderMeshComponent.h>

#include <iInputCommand.h>
#include "physics/interfaces/iRigidBody.h"
#include "cPhysicsComponent.h"
#include "cSkinnedMeshComponent.h"

bool isShiftDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) { return true; }
	// both are up
	return false;
}

bool isCtrlDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { return true; }
	// both are up
	return false;
}

bool isAltDown(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT)) { return true; }
	// both are up
	return false;
}

bool areAllModifiersUp(GLFWwindow* window)
{
	if (isShiftDown(window)) { return false; }
	if (isCtrlDown(window)) { return false; }
	if (isAltDown(window)) { return false; }
	// Yup, they are all UP
	return true;
}

extern float camera_target_y;
void keyboardCallbackASYNC(GLFWwindow* window, float delta_time) {
	/*
	* All keypresses get sent to the "mediator" (scene) and are handled there. The command do no know the
	* actual object they are going to act on until they are executed
	*/

	cScene* pScene = cSceneManager::getInstance()->getActiveScene();

	const float CAMERA_SPEED_SLOW = 0.5f;
	const float CAMERA_SPEED_FAST = 2.5f;

	float cameraSpeed = CAMERA_SPEED_SLOW;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		cameraSpeed = CAMERA_SPEED_FAST;
	}

	cEntity* the_skinned_mesh = cSceneManager::getInstance()->getActiveScene()->getSelectedEntity();
	cPhysicsComponent* physics_component = static_cast<cPhysicsComponent*>(the_skinned_mesh->components[8]);
	physics::interfaces::iRigidBody* body = nullptr;
	glm::vec3 skinned_mesh_forward = glm::vec3(0.0f,0.0f,1.0f);

	if(physics_component)
	{
		body = physics_component->get_rigid_body();
		skinned_mesh_forward = glm::toMat3(body->get_orientation()) * glm::vec3(0.0f, 0.0f, 1.0f);
	}

	cRenderMeshComponent* render_mesh_component = static_cast<cRenderMeshComponent*>(the_skinned_mesh->components[3]);

	float walk_speed;
	float run_speed;
	float strafe_speed;
	if (render_mesh_component->friendlyName == "Boy")
	{
		walk_speed = 20.0f;
		run_speed = 40.0f;
		strafe_speed = 7.5f;
	}
	else
	{
		walk_speed = 15.0f;
		run_speed = 60.0f;
		strafe_speed = 7.5f;
	}

	// If no keys are down, move the camera
	if (areAllModifiersUp(window))
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE || glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
		{
			cSkinnedMeshComponent* animation = static_cast<cSkinnedMeshComponent*>(the_skinned_mesh->components[10]);
			animation->pAniState->defaultAnimation.name = "Idle";

			if(body)
			{
				glm::vec3 cur_vel = body->get_velocity();
				cur_vel = glm::vec3(0.0f,cur_vel.y, 0.0f);
				body->set_velocity(glm::vec3(0.0f));
			}
		}


		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			if(body)
			{
				glm::vec3 cur_vel = body->get_velocity();
				glm::vec3 new_vel = glm::vec3(0.75) * skinned_mesh_forward;
				new_vel.y = cur_vel.y;
				body->set_velocity(new_vel);	
			}
			
			//skin_trans->set_position(skin_trans->get_position() + walk_speed * skinned_mesh_forward * delta_time);
			cSkinnedMeshComponent* animation = static_cast<cSkinnedMeshComponent*>(the_skinned_mesh->components[10]);


			animation->pAniState->defaultAnimation.name = "Walk";
			animation->pAniState->defaultAnimation.frameStepTime += 0.01f;

			//pScene->handleCommand(new cMoveCameraForwardCommand(cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			if(body)
			{
				glm::vec3 cur_vel = body->get_velocity();
				glm::vec3 new_vel = glm::vec3(-0.75) * skinned_mesh_forward;
				new_vel.y = cur_vel.y;
				body->set_velocity(new_vel);	
			}
			
			//skin_trans->set_position(skin_trans->get_position() - walk_speed * skinned_mesh_forward * delta_time);
			cSkinnedMeshComponent* animation = static_cast<cSkinnedMeshComponent*>(the_skinned_mesh->components[10]);

			animation->pAniState->defaultAnimation.name = "Walk Back";
			animation->pAniState->defaultAnimation.frameStepTime += 0.01f;
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			pScene->handleCommand(new cMoveCameraForwardCommand(cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{

			pScene->handleCommand(new cMoveCameraForwardCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			pScene->handleCommand(new cMoveCameraUpDownCommand(cameraSpeed * 3.0f));
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			pScene->handleCommand(new cMoveCameraUpDownCommand(-cameraSpeed * 3.0f));
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			pScene->handleCommand(new cMoveCameraLeftRightCommand(-cameraSpeed * 3.0f));
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			pScene->handleCommand(new cMoveCameraLeftRightCommand(cameraSpeed * 3.0f));
		}
	}

	if (areAllModifiersUp(window))
	{
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			if(body)
			{
				glm::quat cur_ori = body->get_orientation();
				cur_ori *= glm::quat(glm::vec3(glm::radians(0.0), glm::radians(3.0), glm::radians(0.0)));
				body->set_orientation(cur_ori);
			}
			//skin_trans->adjOrientationEulerAngles(glm::vec3(0.0f, 3.0f, 0.0f), true);
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			if(body)
			{
				glm::quat cur_ori = body->get_orientation();
				cur_ori *= glm::quat(glm::vec3(glm::radians(0.0), glm::radians(-3.0), glm::radians(0.0)));
				body->set_orientation(cur_ori);
			}
			//skin_trans->adjOrientationEulerAngles(glm::vec3(0.0f, -3.0f, 0.0f), true);
		}
	}

	if (isShiftDown(window)) {

		cSkinnedMeshComponent* animation = static_cast<cSkinnedMeshComponent*>(the_skinned_mesh->components[10]);
		cTransformComponent* transform_component = static_cast<cTransformComponent*>(the_skinned_mesh->components[1]);

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			if(body)
			{
				glm::quat cur_ori = body->get_orientation();
				cur_ori *= glm::quat(glm::vec3(glm::radians(0.0), glm::radians(3.0), glm::radians(0.0)));
				body->set_orientation(cur_ori);
			}
			//skin_trans->adjOrientationEulerAngles(glm::vec3(0.0f, 3.0f, 0.0f), true);
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			if(body)
			{
				glm::quat cur_ori = body->get_orientation();
				cur_ori *= glm::quat(glm::vec3(glm::radians(0.0), glm::radians(-3.0), glm::radians(0.0)));
				body->set_orientation(cur_ori);
			}
			//skin_trans->adjOrientationEulerAngles(glm::vec3(0.0f, -3.0f, 0.0f), true);
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			//skin_trans->set_position(skin_trans->get_position() + run_speed * skinned_mesh_forward * delta_time);
			cSkinnedMeshComponent* animation = static_cast<cSkinnedMeshComponent*>(the_skinned_mesh->components[10]);


			animation->pAniState->defaultAnimation.name = "Run";
			animation->pAniState->defaultAnimation.frameStepTime += 0.01f;

			if(body)
			{
				glm::vec3 cur_vel = body->get_velocity();
				glm::vec3 new_vel = glm::vec3(5.0) * skinned_mesh_forward;
				new_vel.y = cur_vel.y;
				body->set_velocity(new_vel);	
			}
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			//skin_trans->set_position(skin_trans->get_position() - run_speed * skinned_mesh_forward * delta_time);
			cSkinnedMeshComponent* animation = static_cast<cSkinnedMeshComponent*>(the_skinned_mesh->components[10]);


			animation->pAniState->defaultAnimation.name = "Run Back";
			animation->pAniState->defaultAnimation.frameStepTime += 0.01f;

			if(body)
			{
				glm::vec3 cur_vel = body->get_velocity();
				glm::vec3 new_vel = glm::vec3(-5.0) * skinned_mesh_forward;
				new_vel.y = cur_vel.y;
				body->set_velocity(new_vel);
			}
		}
	}

	if (isAltDown(window)) {

		cTransformComponent* selectedEntityTransformComp = (cTransformComponent*)pScene->getSelectedEntity()->components[1];
		cRenderMeshComponent* selectedEntityMeshComp = (cRenderMeshComponent*)pScene->getSelectedEntity()->components[3];

		cSkinnedMeshComponent* animation = static_cast<cSkinnedMeshComponent*>(the_skinned_mesh->components[10]);
		cTransformComponent* transform_component = static_cast<cTransformComponent*>(the_skinned_mesh->components[1]);

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			if(body)
			{
				glm::vec3 left = body->get_orientation() * glm::vec3(1.0f, 0.0f, 0.0f);
				body->set_position(body->get_position() + 0.75f * left * delta_time);
				animation->pAniState->defaultAnimation.name = "Strafe Left";	
			}
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			if(body)
			{
				glm::vec3 left = body->get_orientation() * glm::vec3(1.0f, 0.0f, 0.0f);
				body->set_position(body->get_position() + -0.75f * left * delta_time);
				animation->pAniState->defaultAnimation.name = "Strafe Right";	
			}
			
		}
	}

}
