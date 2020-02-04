#include <Global.h>
#include <cSceneManager.h>
#include <cShaderManager.h>
#include <cConsole.h>
#include <cTransformComponent.h>
#include <cRenderMeshComponent.h>
#include <cMeshRenderSystem.h>

#include <cDebugRenderer.h>
#include <cEngine.h>

#include <cLightHelper.h>

#include <iInputCommand.h>
#include <physicsShapes.h>
#include <cCollider.h>
#include <cLuaBrain.h>
#include "cFreeTypeRenderer.h"
#include "cPhysicsComponent.h"
#include "cCameraFollowEntity.h"
#include "cSkinnedMeshComponent.h"
#include <glm/gtx/matrix_decompose.inl>
#include "cMazeHelpers.h"
#include "cPathFinding.h"
#include "cInstancedMeshComponent.h"
#include "cParticleSystem.h"
#include "cParticleComponent.h"

extern cEntity* pSphere;
extern cEntity* pCube;
extern sLight* pMovingSpotLight;

extern cAABBTerrianBroadPhaseHierarchy g_AABBTerrianHierarchy;

void showModelNormals();

float randomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void clear_buffer(char* buffer, size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		buffer[i] = '\0';
	}
}

void reset_entity_colors(cEntity* entity)
{
	cScene* scene = cSceneManager::getInstance()->getActiveScene();

	//get the selected entity and the component we need
	cEntity* selected_entity = scene->getSelectedEntity();

	cRenderMeshComponent* mesh_component = static_cast<cRenderMeshComponent*>(entity->components[3]);
	if (!mesh_component)
	{
		return;
	}

	if (entity == selected_entity)
	{
		mesh_component->objColour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else
	{
		mesh_component->objColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}

void render_physics_component_info() {
	cFreeTypeRenderer* text_renderer = cFreeTypeRenderer::getInstance();
	text_renderer->xoffset = 0.0;
	text_renderer->yoffset = 40;

	const int left_column_width = -20;
	const int float_value_column_width = -7;

	cScene* scene = cSceneManager::getInstance()->getActiveScene();
	cEntity* selected_entity = scene->getSelectedEntity();

	//buffer for output
	char out_string[100];

	//id
	sprintf_s(out_string, 100, "%-*s %d", left_column_width, "Selected Entity ID:", selected_entity->UNIQUE_ID);
	text_renderer->renderText(out_string, -1 + text_renderer->xoffset, 1 - text_renderer->yoffset * text_renderer->sy, text_renderer->sx, text_renderer->sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	text_renderer->yoffset += 20;
	clear_buffer(out_string, 100);

	//position
	cPhysicsComponent* physics_info = static_cast<cPhysicsComponent*>(selected_entity->components[8]);
	if (physics_info)
	{
		text_renderer->renderText("Physics Debug Info:", -1 + text_renderer->xoffset, 1 - text_renderer->yoffset * text_renderer->sy, text_renderer->sx, text_renderer->sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		text_renderer->yoffset += 40;

		glm::vec3 current_position = physics_info->get_rigid_body()->get_position();
		sprintf_s(out_string, 100, "%-*s x:%-*.2f y: %-*.2f z: %-*.2f", left_column_width, "Current Position:", float_value_column_width, current_position.x, float_value_column_width, current_position.y, float_value_column_width, current_position.z);
		text_renderer->renderText(out_string, -1 + text_renderer->xoffset, 1 - text_renderer->yoffset * text_renderer->sy, text_renderer->sx, text_renderer->sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		text_renderer->yoffset += 20;
		clear_buffer(out_string, 100);

		//position
		glm::vec3 last_position = physics_info->get_rigid_body()->get_position();
		sprintf_s(out_string, 100, "%-*s x:%-*.2f y: %-*.2f z: %-*.2f", left_column_width, "Last Position:", float_value_column_width, last_position.x, float_value_column_width, last_position.y, float_value_column_width, last_position.z);
		text_renderer->renderText(out_string, -1 + text_renderer->xoffset, 1 - text_renderer->yoffset * text_renderer->sy, text_renderer->sx, text_renderer->sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		text_renderer->yoffset += 20;
		clear_buffer(out_string, 100);

		//acceleration
		glm::vec3 acceleration = physics_info->get_rigid_body()->get_acceleration();
		sprintf_s(out_string, 100, "%-*s x:%-*.2f y: %-*.2f z: %-*.2f", left_column_width, "Acceleration:", float_value_column_width, acceleration.x, float_value_column_width, acceleration.y, float_value_column_width, acceleration.z);
		text_renderer->renderText(out_string, -1 + text_renderer->xoffset, 1 - text_renderer->yoffset * text_renderer->sy, text_renderer->sx, text_renderer->sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		text_renderer->yoffset += 20;
		clear_buffer(out_string, 100);

		//velocity
		glm::vec3 velocity = physics_info->get_rigid_body()->get_velocity();
		sprintf_s(out_string, 100, "%-*s x:%-*.2f y: %-*.2f z: %-*.2f", left_column_width, "Velocity:", float_value_column_width, velocity.x, float_value_column_width, velocity.y, float_value_column_width, velocity.z);
		text_renderer->renderText(out_string, -1 + text_renderer->xoffset, 1 - text_renderer->yoffset * text_renderer->sy, text_renderer->sx, text_renderer->sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		text_renderer->yoffset += 20;
		clear_buffer(out_string, 100);
	}
}

//todo: actual collision check
bool is_collision_with_hands()
{
	return false;
}


float camera_target_y = 5.0f;
float min_follow_distance = 5.0f;
float current_target_follow_distance = 5.0f;
float max_follow_distance = 100.0f;
void camera_follow_selected_entity(float delta_time)
{
	//get the active scene
	cScene* scene = cSceneManager::getInstance()->getActiveScene();

	//get the selected entity and the component we need

	cEntity* selected_entity = scene->getSelectedEntity();
	cPhysicsComponent* transform = static_cast<cPhysicsComponent*>(selected_entity->components[8]);

	if (!transform)
	{
		return;
	}


	cFlyCamera* camera = &scene->flyCamera;

	//get the positions of the camera and selected entity
	glm::vec3 entity_pos = transform->get_rigid_body()->get_position();
	glm::vec3 camera_pos = camera->eye;

	//calculate distance and direction
	float distance = glm::distance(entity_pos, camera_pos);
	glm::vec3 direction_to_entity = glm::normalize(entity_pos - camera_pos);
	glm::vec3 target_location = entity_pos + (transform->get_rigid_body()->get_orientation() * glm::vec3(0.0, 0.0, -5.5));
	glm::vec3 camera_direction_to_target = glm::normalize(target_location - camera_pos);

	glm::vec3 newPos = camera->eye;

	//move towards the target
	newPos += camera_direction_to_target * 15.0f * delta_time;

	newPos.y = entity_pos.y + 0.75f;
	camera->eye = newPos;

	//face the selected entity
	direction_to_entity.y = 0.0f;
	glm::quat new_camera_orientation = glm::quat(glm::inverse(glm::lookAt(camera->eye, camera->eye - direction_to_entity, glm::vec3(0.0f, 1.0f, 0.0f))));

	camera->setMeshOrientationQ(new_camera_orientation);
}

void render_camera_info()
{
	cFreeTypeRenderer* text_renderer = cFreeTypeRenderer::getInstance();
	text_renderer->yoffset = 40;
	text_renderer->xoffset = 1.1f;

	const int left_column_width = -10;
	const int float_value_column_width = -7;

	cScene* scene = cSceneManager::getInstance()->getActiveScene();

	//buffer for output
	char out_string[100];

	sprintf_s(out_string, 100, "%-*s", left_column_width, "Camera Info:");
	text_renderer->renderText(out_string, -1 + text_renderer->xoffset, 1 - text_renderer->yoffset * text_renderer->sy, text_renderer->sx, text_renderer->sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	text_renderer->yoffset += 60;
	clear_buffer(out_string, 100);


	glm::vec3 current_position = scene->flyCamera.eye;
	sprintf_s(out_string, 100, "%-*s x:%-*.2f y: %-*.2f z: %-*.2f", left_column_width, "Position:", float_value_column_width, current_position.x, float_value_column_width, current_position.y, float_value_column_width, current_position.z);
	text_renderer->renderText(out_string, -1 + text_renderer->xoffset, 1 - text_renderer->yoffset * text_renderer->sy, text_renderer->sx, text_renderer->sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	text_renderer->yoffset += 20;
	clear_buffer(out_string, 100);

	glm::vec3 at = scene->flyCamera.getAtInWorldSpace();
	sprintf_s(out_string, 100, "%-*s x:%-*.2f y: %-*.2f z: %-*.2f", left_column_width, "At:", float_value_column_width, at.x, float_value_column_width, at.y, float_value_column_width, at.z);
	text_renderer->renderText(out_string, -1 + text_renderer->xoffset, 1 - text_renderer->yoffset * text_renderer->sy, text_renderer->sx, text_renderer->sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	text_renderer->yoffset += 20;
	clear_buffer(out_string, 100);

	glm::vec3 up = scene->flyCamera.getUpVector();
	sprintf_s(out_string, 100, "%-*s x:%-*.2f y: %-*.2f z: %-*.2f", left_column_width, "Up:", float_value_column_width, up.x, float_value_column_width, up.y, float_value_column_width, up.z);
	text_renderer->renderText(out_string, -1 + text_renderer->xoffset, 1 - text_renderer->yoffset * text_renderer->sy, text_renderer->sx, text_renderer->sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	text_renderer->yoffset += 20;
	clear_buffer(out_string, 100);

	glm::quat quat = scene->flyCamera.getQOrientation();
	sprintf_s(out_string, 100, "%-*s x:%-*.2f y: %-*.2f z: %-*.2f w: %-*.2f", left_column_width, "Quat:", float_value_column_width, quat.x, float_value_column_width, quat.y, float_value_column_width, quat.z, float_value_column_width, quat.w);
	text_renderer->renderText(out_string, -1 + text_renderer->xoffset, 1 - text_renderer->yoffset * text_renderer->sy, text_renderer->sx, text_renderer->sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	text_renderer->yoffset += 20;
	clear_buffer(out_string, 100);
}

extern cInstancedMeshComponent* instance_renderer;
extern std::vector<fire_info> fires;
void updateCallback(double deltaTime) {

	if (cSceneManager::getInstance()->getActiveScene()->follow_cam_enabled)
	{
		camera_follow_selected_entity(deltaTime);
	}

	//player
	cEntity* player = cSceneManager::getInstance()->getActiveScene()->getSelectedEntity();
	cPhysicsComponent* player_transform = static_cast<cPhysicsComponent*>(player->components[8]);
	int fires_left_to_activate = 0;

	for (int i = 0; i < fires.size(); ++i)
	{
		glm::vec3 fire_pos = static_cast<cTransformComponent*>(fires[i].smoke->components[1])->get_position();
		glm::vec3 player_pos = player_transform->get_rigid_body()->get_position();
		if (glm::abs(glm::distance(fire_pos, player_pos)) < 1.0f)
		{
			static_cast<cParticleComponent*>(fires[i].smoke->components[7])->particleGenerationOn();
			static_cast<cParticleComponent*>(fires[i].fire->components[7])->particleGenerationOn();
		}

		//is the fire activated
		if (!static_cast<cParticleComponent*>(fires[i].smoke->components[7])->is_activated())
		{
			fires_left_to_activate += 1;
		}
	}


	cFreeTypeRenderer* text_renderer = cFreeTypeRenderer::getInstance();
	text_renderer->yoffset = 60;
	text_renderer->xoffset = 0.2f;

	const int left_column_width = -10;

	cScene* scene = cSceneManager::getInstance()->getActiveScene();

	//buffer for output
	char out_string[100];

	sprintf_s(out_string, 100, "%-*s %d", left_column_width, "Fires Left To Light:", fires_left_to_activate);
	text_renderer->renderText(out_string, -1 + text_renderer->xoffset, 1 - text_renderer->yoffset * text_renderer->sy, text_renderer->sx, text_renderer->sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	text_renderer->yoffset += 60;
	clear_buffer(out_string, 100);

}


