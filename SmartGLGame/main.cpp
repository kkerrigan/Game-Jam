
#include <iostream>
#include <cEngine.h>
#include <cConsole.h>
#include <cShaderManager.h>
#include <cVAOMeshManager.h>
#include <cEntityBuilder.h>
#include <cDebugRenderer.h>
#include <cSoundManager.h>
#include "cFreeTypeRenderer.h"
#include "cSerializer.h"
#include "cLoadBitmapMap.h"
#include "physics/defs/sHeightmapDef.h"
#include "CTextureFromBMP.h"
#include "cBasicTextureManager.h"
#include "cPhysicsComponent.h"
#include "cScene.h"
#include "cSceneManager.h"
#include <random>
#include <ctime>
#include "cInstancedMeshComponent.h"
#include "cMeshRenderSystem.h"

// forward declare some functions
bool loadShaders();
void loadTextures();

void updateCallback(double deltaTime);

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void keyboardCallbackASYNC(GLFWwindow* window, float delta_time);

void mouseCallbackAsync(GLFWwindow* window);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_enter_callback(GLFWwindow* window, int entered);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void window_size_callback(GLFWwindow* window, int width, int height);

void loadModels();
void loadScene();
void loadLights();
void loadTerrainAABBHierarchy();
bool load_cutscene();

DWORD WINAPI DalekBrainThread(void* pInitialData);

void add_trees_to_scene(int number_of_trees)
{
	srand(time(NULL));
	sModelDrawInfo draw_info;
	draw_info.meshFileName = "island.ply";
	cVAOMeshManager::getInstance()->FindDrawInfoByModelName(draw_info);
	CTextureFromBMP* terrain_splat_1_data = cBasicTextureManager::getInstance()->m_map_TexNameToTexture["island_splat.bmp"];	//for the terrain type (sand/grass)
	cScene* scene = cSceneManager::getInstance()->getActiveScene();

	cEntity* instanced_tree_bots_1 = cEntityBuilder::getInstance()->createEntity(5);
	cInstancedMeshComponent* instanced_mesh_component_tree_bot_1 = static_cast<cInstancedMeshComponent*>(instanced_tree_bots_1->components[12]);
	cRenderMeshComponent* render_mesh_component = static_cast<cRenderMeshComponent*>(instanced_tree_bots_1->components[3]);
	render_mesh_component->meshName = "tree_base1.ply";
	render_mesh_component->textures[0].name = "bark.bmp";

	cEntity* instanced_tree_bots_2 = cEntityBuilder::getInstance()->createEntity(5);
	cInstancedMeshComponent* instanced_mesh_component_tree_bot_2 = static_cast<cInstancedMeshComponent*>(instanced_tree_bots_2->components[12]);
	cRenderMeshComponent* render_mesh_component2 = static_cast<cRenderMeshComponent*>(instanced_tree_bots_2->components[3]);
	render_mesh_component2->meshName = "tree_base2.ply";
	render_mesh_component2->textures[0].name = "bark.bmp";

	cEntity* instanced_tree_tops_1 = cEntityBuilder::getInstance()->createEntity(5);
	cInstancedMeshComponent* instanced_mesh_component_tree_top_1 = static_cast<cInstancedMeshComponent*>(instanced_tree_tops_1->components[12]);
	cRenderMeshComponent* render_mesh_component3 = static_cast<cRenderMeshComponent*>(instanced_tree_tops_1->components[3]);
	render_mesh_component3->meshName = "tree_top1.ply";
	render_mesh_component3->textures[0].name = "leaves.bmp";

	cEntity* instanced_tree_tops_2 = cEntityBuilder::getInstance()->createEntity(5);
	cInstancedMeshComponent* instanced_mesh_component_tree_top_2 = static_cast<cInstancedMeshComponent*>(instanced_tree_tops_2->components[12]);
	cRenderMeshComponent* render_mesh_component4 = static_cast<cRenderMeshComponent*>(instanced_tree_tops_2->components[3]);
	render_mesh_component4->meshName = "tree_top2.ply";
	render_mesh_component4->textures[0].name = "leaves.bmp";

	cEntity* instanced_tree_tops_3 = cEntityBuilder::getInstance()->createEntity(5);
	cInstancedMeshComponent* instanced_mesh_component_tree_top_3 = static_cast<cInstancedMeshComponent*>(instanced_tree_tops_3->components[12]);
	cRenderMeshComponent* render_mesh_component5 = static_cast<cRenderMeshComponent*>(instanced_tree_tops_3->components[3]);
	render_mesh_component5->meshName = "tree_top3.ply";
	render_mesh_component5->textures[0].name = "leaves.bmp";

	cEntity* instanced_tree_tops_4 = cEntityBuilder::getInstance()->createEntity(5);
	cInstancedMeshComponent* instanced_mesh_component_tree_top_4 = static_cast<cInstancedMeshComponent*>(instanced_tree_tops_4->components[12]);
	cRenderMeshComponent* render_mesh_component6 = static_cast<cRenderMeshComponent*>(instanced_tree_tops_4->components[3]);
	render_mesh_component6->meshName = "tree_top4.ply";
	render_mesh_component6->textures[0].name = "leaves.bmp";

	std::default_random_engine generator;
	std::uniform_int_distribution<unsigned> distribution(0, draw_info.numberOfVertices);

	for (int i = 0; i < number_of_trees; ++i)
	{
		//pick a random vertex on the map
		unsigned int random_vert = distribution(generator);
		sVertex_xyz_rgba_n_uv2_bt_4Bones vertex_data = draw_info.pMeshData->pVertices[random_vert];

		//check that is is on the grass (splat map)
		float hor1 = (vertex_data.x + 256.0) / 512.0f * 512.0f;
		float ver1 = (vertex_data.z + 256.0) / 512.0f * 512.0f;
		auto terrain_splat = terrain_splat_1_data->getPixelAtRowColumn(hor1, ver1);

		//blue is grass
		if (terrain_splat.bluePixel >= 0.5f)
		{
			int tree_base_type = rand() % 2;
			int tree_top_type = rand() % 4;

			std::string tree_base_string;
			std::string tree_top_string;

			switch (tree_base_type)
			{
			case 0:
				tree_base_string = "tree_base1.ply";
				break;
			case 1:
				tree_base_string = "tree_base2.ply";
				break;
			}

			switch (tree_top_type)
			{
			case 0:
				tree_top_string = "tree_top1.ply";
				break;
			case 1:
				tree_top_string = "tree_top2.ply";
				break;					
			case 2:						
				tree_top_string = "tree_top3.ply";
				break;					
			case 3:						
				tree_top_string = "tree_top4.ply";
				break;
			}

			cEntity* tree_base = cEntityBuilder::getInstance()->createEntity(0);
			cPhysicsComponent* transform_component = static_cast<cPhysicsComponent*>(tree_base->components[8]);
			cMeshRenderSystem::getInstance()->unregisterEntity(tree_base);
			float growth_factor = rand()/static_cast<float>(RAND_MAX);
			float scale_factor = 0.25f;

			physics::defs::sRigidBodyDef physics_def;
			physics_def.shape_type = physics::interfaces::iShape::shape_types::capsule;
			physics_def.height = 11.6f * scale_factor;
			physics_def.radius = 0.20f;
			physics_def.mass = 0.0f;

			physics::interfaces::iRigidBody* body = cEngine::getInstance()->get_physics_factory()->create_rigid_body(physics_def);
			transform_component->configure_component(body);

			body->set_position(glm::vec3(
				vertex_data.x,
				vertex_data.y,
				vertex_data.z
			));

			body->set_scale(glm::vec3(scale_factor));

			cEngine::getInstance()->get_world()->register_rigid_body(body);

			cRenderMeshComponent* mesh_component = static_cast<cRenderMeshComponent*>(tree_base->components[3]);
			mesh_component->objColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			mesh_component->meshName = tree_base_string;
			mesh_component->friendlyName = "tree1";
			mesh_component->bIsWireFrame = false;
			mesh_component->bDontLight = false;
			mesh_component->specular = glm::vec4(1.0f, 1.0f, 1.0f, 10.0f);
			mesh_component->bIsVisible = false;
			sTextureInfo info;
			info.name = "bark.bmp";
			info.strength = 1.0f;
			mesh_component->textures.push_back(info);

			cInstancedMeshComponent::mesh_instance mesh_instance;
			mesh_instance.position = body->get_position();
			mesh_instance.orientation = body->get_orientation();

			switch (tree_base_type)
			{
			case 0:
				instanced_mesh_component_tree_bot_1->instances.push_back(mesh_instance);
				break;
			case 1:
				instanced_mesh_component_tree_bot_2->instances.push_back(mesh_instance);
				break;
			}


			cEntity* tree_top = cEntityBuilder::getInstance()->createEntity(1);
			cTransformComponent* transform_component2 = static_cast<cTransformComponent*>(tree_top->components[1]);
			transform_component2->set_position(glm::vec3(
				vertex_data.x,
				vertex_data.y + 2.0f,
				vertex_data.z
			));
			transform_component2->scale = glm::vec3(0.25f);
			transform_component2->setOrientationEulerAngles(glm::vec3(0.0f, (rand() % 360),0.0f), true);
			cMeshRenderSystem::getInstance()->unregisterEntity(tree_top);

			cRenderMeshComponent* mesh_component2 = static_cast<cRenderMeshComponent*>(tree_top->components[3]);
			mesh_component2->objColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			mesh_component2->meshName = tree_top_string;
			mesh_component2->friendlyName = "tree";
			mesh_component2->bIsWireFrame = false;
			mesh_component2->bDontLight = false;
			mesh_component2->bIsVisible = false;
			mesh_component2->specular = glm::vec4(1.0f, 1.0f, 1.0f, 10000.0f);

			mesh_instance.position = transform_component2->get_position();
			mesh_instance.orientation = transform_component2->getQOrientation();

			switch (tree_top_type)
			{
			case 0:
				instanced_mesh_component_tree_top_1->instances.push_back(mesh_instance);
				break;
			case 1:
				instanced_mesh_component_tree_top_2->instances.push_back(mesh_instance);
				break;					
			case 2:						
				instanced_mesh_component_tree_top_3->instances.push_back(mesh_instance);
				break;					
			case 3:						
				instanced_mesh_component_tree_top_4->instances.push_back(mesh_instance);
				break;
			}

			sTextureInfo info2;
			info2.name = "leaves.bmp";
			info2.strength = 1.0f;
			mesh_component2->textures.push_back(info2);
		}
		else
		{
			--i; //we didn't make a tree
		}
	}
}

int main() {
	cConsole console;
	console.log(0, "Before the Village - by Daniel Maclam");

	// initialize the engine and window
	cEngine* pEngine = cEngine::getInstance();
	sWindowSettings* windowSettings = new sWindowSettings();

	cSerializer::load("./assets/settings/settings.json", windowSettings);

	if (!pEngine->initialize(windowSettings)) {
		std::cout << "Failed to initialize engine." << std::endl;
		exit(EXIT_FAILURE);
	}

	if (!loadShaders()) {
		std::cout << "Failed to load shaders." << std::endl;
		exit(EXIT_FAILURE);
	}

	//initialize freetype
	cFreeTypeRenderer* text_renderer = cFreeTypeRenderer::getInstance();
	text_renderer->initText(16);
	text_renderer->getTextUniforms();

	// Set up SOund Manager
	cSoundManager* pSoundManager = cSoundManager::getInstance();
	pSoundManager->initialize();
	pSoundManager->setBasePath("./assets/sounds/");

	// set lots of callbacks
	pEngine->setUpdateCallback(updateCallback);
	pEngine->setKeyboardCallback(keyboardCallback);
	pEngine->setKeyboardCallbackAsync(keyboardCallbackASYNC);
	pEngine->setMouseCallbackAsync(mouseCallbackAsync);
	pEngine->setMouseCursorPositionCallback(cursor_position_callback);
	pEngine->setMouseButtonCallback(mouse_button_callback);
	pEngine->setMouseEnterCallback(cursor_enter_callback);
	pEngine->setMouseScrollCallback(scroll_callback);
	pEngine->setResizeCallback(window_size_callback);

	// setup debug renderer
	cDebugRenderer::getInstance()->initialize();

	//load textures
	loadTextures();

	//load model files
	loadModels();

	// load the entities
	loadScene();

	// load the terrain aabb Hierarchy (for broad phase collision detection)
	//loadTerrainAABBHierarchy();

	dsm::sHeightmapDef terrain_def;

	terrain_def.height = 256;
	terrain_def.width = 256;
	terrain_def.min_height = 0.0f;
	terrain_def.max_height = 20000.0f;
	terrain_def.scale = 5.0f;

	sModelDrawInfo draw_info;
	draw_info.meshFileName = "island.ply";
	cVAOMeshManager::getInstance()->FindDrawInfoByModelName(draw_info);

	//load the heightmap for the terrain and change the vertices for physics
	//get the vertices from the model manager
	unsigned int vertexBufferSizeInBytes = sizeof(sVertex_xyz_rgba_n_uv2_bt_4Bones) * draw_info.numberOfVertices;

	//get the texture
	CTextureFromBMP* terain_texture_1_data = cBasicTextureManager::getInstance()->m_map_TexNameToTexture["island.bmp"];
	CTextureFromBMP* terrain_normal_data = cBasicTextureManager::getInstance()->m_map_TexNameToTexture["island_normal.bmp"];

	for (auto vert_index = 0; vert_index < draw_info.numberOfTriangles; ++vert_index)
	{
		float hor1 = (draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_0].x + 256.0) / 512.0f * 512.0f;
		float ver1 = (draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_0].z + 256.0) / 512.0f * 512.0f;
		auto temp1 = terain_texture_1_data->getPixelAtRowColumn(hor1, ver1);

		float hor2 = (draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_1].x + 256.0) / 512.0f * 512.0f;
		float ver2 = (draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_1].z + 256.0) / 512.0f * 512.0f;
		auto temp2 = terain_texture_1_data->getPixelAtRowColumn(hor2, ver2);

		float hor3 = (draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_2].x + 256.0) / 512.0f * 512.0f;
		float ver3 = (draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_2].z + 256.0) / 512.0f * 512.0f;
		auto temp3 = terain_texture_1_data->getPixelAtRowColumn(hor3, ver3);

		//vertices
		draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_0].y = ((float)temp1.redPixel / 255.0f) * 20;
		draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_1].y = ((float)temp2.redPixel / 255.0f) * 20;
		draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_2].y = ((float)temp3.redPixel / 255.0f) * 20;

		//texture coordinates
		draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_0].u0 = ver1 / 512.0f;
		draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_0].u1 = hor1 / 512.0f;
		draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_1].u0 = ver2 / 512.0f;
		draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_1].u1 = hor2 / 512.0f;
		draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_2].u0 = ver3 / 512.0f;
		draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_2].u1 = hor3 / 512.0f;

		//normals
		auto normal_v1 = terrain_normal_data->getPixelAtRowColumn(hor1, ver1);
		auto normal_v2 = terrain_normal_data->getPixelAtRowColumn(hor2, ver2);
		auto normal_v3 = terrain_normal_data->getPixelAtRowColumn(hor3, ver3);

		draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_0].nx = ((float)normal_v1.redPixel / 255.0f);
		draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_0].ny = ((float)normal_v1.greenPixel / 255.0f);
		draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_0].nw = ((float)normal_v1.bluePixel / 255.0f);

		draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_1].nx = ((float)normal_v2.redPixel / 255.0f);
		draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_1].ny = ((float)normal_v2.greenPixel / 255.0f);
		draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_1].nw = ((float)normal_v2.bluePixel / 255.0f);

		draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_2].nx = ((float)normal_v3.redPixel / 255.0f);
		draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_2].ny = ((float)normal_v3.greenPixel / 255.0f);
		draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[vert_index].vertex_ID_2].nw = ((float)normal_v3.bluePixel / 255.0f);
	}

	glBindVertexArray(draw_info.VAO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, draw_info.vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferSizeInBytes, draw_info.pMeshData->pVertices, GL_DYNAMIC_DRAW);

	//load the heightmap transformed data for the island here (for physics)
	for (int i = 0; i < draw_info.pMeshData->numberOfTriangles; ++i)
	{
		dsm::sHeightmapVertex vert;

		vert.vertexes[0] = glm::vec3(draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[i].vertex_ID_0].x, draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[i].vertex_ID_0].y, draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[i].vertex_ID_0].z);
		vert.vertexes[1] = glm::vec3(draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[i].vertex_ID_1].x, draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[i].vertex_ID_1].y, draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[i].vertex_ID_1].z);
		vert.vertexes[2] = glm::vec3(draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[i].vertex_ID_2].x, draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[i].vertex_ID_2].y, draw_info.pMeshData->pVertices[draw_info.pMeshData->pTriangles[i].vertex_ID_2].z);

		terrain_def.data.push_back(vert);
	}

	add_trees_to_scene(1500);

	//cleanup some memory after using texture data (c++ side)
	cBasicTextureManager::getInstance()->post_load_cleanup();

	pEngine->get_world()->register_heightmap_terrain(pEngine->get_physics_factory()->create_height_map_terrain(terrain_def));

	// start the game loop (hang on..!)
	pEngine->startGameLoop();

	cSoundManager::getInstance()->shutDown();

	return 0;
}
