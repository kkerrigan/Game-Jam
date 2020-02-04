#include <cEntityBuilder.h>
#include <cSceneManager.h>
#include <cLightManager.h>
#include <cColliderSystem.h>
#include <cLuaBrain.h>
#include "cScriptingCommandSystem.h"
#include "cFrameBufferComponent.h"
#include "cThreadWorkerComponent.h"
#include "cLoadBitmapMap.h"
#include "cInstancedMeshComponent.h"
#include "cPhysicsComponent.h"
#include "cPathFinding.h"
#include "cComponentFactory.h"
#include "cBehaviourComponent.h"
#include "cFollowPathBehaviour.h"
#include "cBehaviourSystem.h"
#include "cGathererBehaviour.h"
#include "cEngine.h"
#include "cParticleComponent.h"
#include "cParticleSystem.h"

cEntity* pSphere = nullptr;
cEntity* pCube = nullptr;

sLight* pMovingSpotLight = nullptr;

extern std::map<std::string, cCommandGroup*> commandGroups;
extern std::map<std::string, iCommand*> commands;

std::vector<cEntity*> fbos;

std::vector<fire_info> fires;
std::vector<glm::vec3> the_found_path;
cInstancedMeshComponent* instance_renderer;
void loadScene() {

	// create a scene for the entities
	cSceneManager* pSceneManager = cSceneManager::getInstance();

	cScene* pMazeScene = pSceneManager->createScene("sandbox.json");
	pSceneManager->setActiveScene("sandbox.json");

	//Animation Midterm - Question # 3
	//The skin mesh animated model 
	

	cEntity* skinMesh2 = cEntityBuilder::getInstance()->createEntity(4);
	//static_cast<cTransformComponent*>(skinMesh2->components[1])->set_position(glm::vec3(-256.0, 0.0f, 0.0f));
	pMazeScene->addEntityToScene(skinMesh2);

	
	//initialize the light manager before loading them
	cLightManager::getInstance()->initializeLights();

	//Graphics Midterm - Question # 4
	//this is the main offscreen fbo
	cEntity* entity = cEntityBuilder::getInstance()->createEntity(2);
	static_cast<cRenderMeshComponent*>(entity->components[3])->friendlyName = "quad";
	static_cast<cFrameBufferComponent*>(entity->components[9])->inititFrameBuffer(0);
	fbos.push_back(entity);
	pMazeScene->addEntityToScene(entity);

	//load from the file
	pMazeScene->deserialize("maze.json");

	{
		cEntity* water = cEntityBuilder::getInstance()->createEntity(1);
		cTransformComponent* transform_component = static_cast<cTransformComponent*>(water->components[1]);
		transform_component->set_position(glm::vec3(0.0f, 7.0, 0.0f));
		transform_component->scale = glm::vec3(30.0f);
		cRenderMeshComponent* mesh_component = static_cast<cRenderMeshComponent*>(water->components[3]);
		mesh_component->objColour = glm::vec4(0.0f, 0.0f, 0.5f, 0.5f);
		mesh_component->meshName = "flat.ply";
		mesh_component->friendlyName = "water";
		mesh_component->bIsWireFrame =false;
		mesh_component->bUseWaterHeightMaps = true;
		mesh_component->specular = glm::vec4(1.0f,1.0f,1.0f,10000.0f);

		sTextureInfo info;
		info.name = "water1.bmp";
		info.strength = 0.25f;
		mesh_component->textures.push_back(info);
		pMazeScene->addEntityToScene(water);
	}

	{
		cEntity* building = cEntityBuilder::getInstance()->createEntity(1);
		cTransformComponent* transform_component = static_cast<cTransformComponent*>(building->components[1]);
		transform_component->set_position(glm::vec3(-130.0, 1.25, -10.48f));
		transform_component->scale = glm::vec3(0.05f);
		cRenderMeshComponent* mesh_component = static_cast<cRenderMeshComponent*>(building->components[3]);
		mesh_component->objColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		mesh_component->meshName = "b1.ply";
		mesh_component->friendlyName = "building";
		mesh_component->bIsWireFrame =false; 
		mesh_component->specular = glm::vec4(1.0f,1.0f,1.0f,10000.0f);

		sTextureInfo info;
		info.name = "debug_grid.bmp";
		info.strength = 1.0f;
		mesh_component->textures.push_back(info);
		pMazeScene->addEntityToScene(building);
	}


	fire_info fire_1;
	{
		cEntity* fire_pit = cEntityBuilder::getInstance()->createEntity(1);
		fire_1.smoke = fire_pit;
		cTransformComponent* transform_component = static_cast<cTransformComponent*>(fire_pit->components[1]);
		transform_component->set_position(glm::vec3(-130.0, 2.25, -5.48f));
		transform_component->scale = glm::vec3(0.5f);
		cRenderMeshComponent* mesh_component = static_cast<cRenderMeshComponent*>(fire_pit->components[3]);
		mesh_component->objColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		mesh_component->meshName = "firepit.ply";
		mesh_component->friendlyName = "firepit";
		mesh_component->bIsWireFrame =false; 
		mesh_component->specular = glm::vec4(1.0f,1.0f,1.0f,10000.0f);

		sTextureInfo info;
		info.name = "bark.bmp";
		info.strength = 1.0f;
		mesh_component->textures.push_back(info);
		pMazeScene->addEntityToScene(fire_pit);

		fire_pit->components[7] = new cParticleComponent();
		cParticleComponent* particle_component = static_cast<cParticleComponent*>(fire_pit->components[7]);


		cEntity* particle = cEntityBuilder::getInstance()->createEntity(1);
		cRenderMeshComponent* particle_mesh = static_cast<cRenderMeshComponent*>(particle->components[3]);
		static_cast<cTransformComponent*>(particle->components[1])->scale = glm::vec3(0.1f);
		particle_mesh->meshName = "1x1_Star_1_Quad_2_Sided_xyz_n_uv.ply";
		particle_mesh->bIsWireFrame = false;
		particle_mesh->bDontLight = false;

		sTextureInfo tex_info;
		tex_info.name = "smoke.bmp";
		tex_info.strength = 1.0f;
		particle_mesh->textures.push_back(tex_info);
		particle_mesh->objColour = glm::vec4(0.1f,0.1,0.1f,1.0f);

		particle_component->particleEntity = particle;

		cParticleComponent::sParticleCreationParams particle_params;
		particle_params.position = glm::vec3(0.0, 0.25, 0.0);
		particle_params.minInitialVelocity = glm::vec3(-0.1,0.5f,-0.1f);
		particle_params.maxInitialVelocity = glm::vec3(0.1,1.0f,0.1f);
		particle_params.minLifeTime = 0.8;
		particle_params.maxLifeTime = 2.0;
		particle_params.minNumberNewParticles = 0;
		particle_params.maxNumberNewParticles = 2;
		particle_params.minOrientationChangeEuler = glm::vec3(-0.60);
		particle_params.maxOrientationChangeEuler = glm::vec3(0.60);
		particle_params.maxInitialScale = 0.015000000596046449f;
		particle_params.maxScaleChange = 0.001f;
		particle_params.deathTransparencyFadeTimeSeconds = 2.0f;

		cParticleSystem::getInstance()->registerEntity(fire_pit);

		particle_component->init(particle_params);
		particle_component->reset(100,false);
	}

	{
		cEntity* fire_pit = cEntityBuilder::getInstance()->createEntity(1);
		fire_1.fire = fire_pit;
		cTransformComponent* transform_component = static_cast<cTransformComponent*>(fire_pit->components[1]);
		transform_component->set_position(glm::vec3(-130.0, 2.25, -5.48f));
		transform_component->scale = glm::vec3(0.5f);
		cRenderMeshComponent* mesh_component = static_cast<cRenderMeshComponent*>(fire_pit->components[3]);
		mesh_component->objColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		mesh_component->meshName = "firepit.ply";
		mesh_component->friendlyName = "firepit";
		mesh_component->bIsWireFrame =false; 
		mesh_component->specular = glm::vec4(1.0f,1.0f,1.0f,10000.0f);

		sTextureInfo info;
		info.name = "debug_grid.bmp";
		info.strength = 1.0f;
		mesh_component->textures.push_back(info);
		pMazeScene->addEntityToScene(fire_pit);

		fire_pit->components[7] = new cParticleComponent();
		cParticleComponent* particle_component = static_cast<cParticleComponent*>(fire_pit->components[7]);


		cEntity* particle = cEntityBuilder::getInstance()->createEntity(1);
		cRenderMeshComponent* particle_mesh = static_cast<cRenderMeshComponent*>(particle->components[3]);
		static_cast<cTransformComponent*>(particle->components[1])->scale = glm::vec3(0.1f);
		particle_mesh->meshName = "1x1_Star_1_Quad_2_Sided_xyz_n_uv.ply";
		particle_mesh->bIsWireFrame = false;
		particle_mesh->bDontLight = false;

		sTextureInfo tex_info;
		tex_info.name = "fire.bmp";
		tex_info.strength = 1.0f;
		particle_mesh->textures.push_back(tex_info);
		particle_mesh->objColour = glm::vec4(1.0f,0.0,0.0f,1.0f);

		particle_component->particleEntity = particle;

		cParticleComponent::sParticleCreationParams particle_params;
		particle_params.position = glm::vec3(0.0, 0.0, 0.0);
		particle_params.minInitialVelocity = glm::vec3(0.0,0.5f,0.0f);
		particle_params.maxInitialVelocity = glm::vec3(0.2,0.8f,0.2f);
		particle_params.minLifeTime = 0.1;
		particle_params.maxLifeTime = 0.3;
		particle_params.minNumberNewParticles = 0;
		particle_params.maxNumberNewParticles = 2;
		particle_params.minOrientationChangeEuler = glm::vec3(-0.60);
		particle_params.maxOrientationChangeEuler = glm::vec3(0.60);
		particle_params.maxInitialScale = 0.015000000596046449f;
		particle_params.maxScaleChange = 0.001f;
		particle_params.deathTransparencyFadeTimeSeconds = 2.0f;

		cParticleSystem::getInstance()->registerEntity(fire_pit);

		particle_component->init(particle_params);
		particle_component->reset(100,false);
	}
	fires.push_back(fire_1);

	fire_info fire_2;
	{
		cEntity* fire_pit = cEntityBuilder::getInstance()->createEntity(1);
		fire_2.smoke = fire_pit;
		cTransformComponent* transform_component = static_cast<cTransformComponent*>(fire_pit->components[1]);
		transform_component->set_position(glm::vec3(-56.0, 2.2,97.48f));
		transform_component->scale = glm::vec3(0.5f);
		cRenderMeshComponent* mesh_component = static_cast<cRenderMeshComponent*>(fire_pit->components[3]);
		mesh_component->objColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		mesh_component->meshName = "firepit.ply";
		mesh_component->friendlyName = "firepit";
		mesh_component->bIsWireFrame =false; 
		mesh_component->specular = glm::vec4(1.0f,1.0f,1.0f,10000.0f);

		sTextureInfo info;
		info.name = "bark.bmp";
		info.strength = 1.0f;
		mesh_component->textures.push_back(info);
		pMazeScene->addEntityToScene(fire_pit);

		fire_pit->components[7] = new cParticleComponent();
		cParticleComponent* particle_component = static_cast<cParticleComponent*>(fire_pit->components[7]);


		cEntity* particle = cEntityBuilder::getInstance()->createEntity(1);
		cRenderMeshComponent* particle_mesh = static_cast<cRenderMeshComponent*>(particle->components[3]);
		static_cast<cTransformComponent*>(particle->components[1])->scale = glm::vec3(0.1f);
		particle_mesh->meshName = "1x1_Star_1_Quad_2_Sided_xyz_n_uv.ply";
		particle_mesh->bIsWireFrame = false;
		particle_mesh->bDontLight = false;

		sTextureInfo tex_info;
		tex_info.name = "smoke.bmp";
		tex_info.strength = 1.0f;
		particle_mesh->textures.push_back(tex_info);
		particle_mesh->objColour = glm::vec4(0.1f,0.1,0.1f,1.0f);

		particle_component->particleEntity = particle;

		cParticleComponent::sParticleCreationParams particle_params;
		particle_params.position = glm::vec3(0.0, 0.25, 0.0);
		particle_params.minInitialVelocity = glm::vec3(-0.1,0.5f,-0.1f);
		particle_params.maxInitialVelocity = glm::vec3(0.1,1.0f,0.1f);
		particle_params.minLifeTime = 0.8;
		particle_params.maxLifeTime = 2.0;
		particle_params.minNumberNewParticles = 0;
		particle_params.maxNumberNewParticles = 2;
		particle_params.minOrientationChangeEuler = glm::vec3(-0.60);
		particle_params.maxOrientationChangeEuler = glm::vec3(0.60);
		particle_params.maxInitialScale = 0.015000000596046449f;
		particle_params.maxScaleChange = 0.001f;
		particle_params.deathTransparencyFadeTimeSeconds = 2.0f;

		cParticleSystem::getInstance()->registerEntity(fire_pit);

		particle_component->init(particle_params);
		particle_component->reset(100,false);
	}

	{
		cEntity* fire_pit = cEntityBuilder::getInstance()->createEntity(1);
		fire_2.fire = fire_pit;
		cTransformComponent* transform_component = static_cast<cTransformComponent*>(fire_pit->components[1]);
		transform_component->set_position(glm::vec3(-56.0, 2.2,97.48f));
		transform_component->scale = glm::vec3(0.5f);
		cRenderMeshComponent* mesh_component = static_cast<cRenderMeshComponent*>(fire_pit->components[3]);
		mesh_component->objColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		mesh_component->meshName = "firepit.ply";
		mesh_component->friendlyName = "firepit";
		mesh_component->bIsWireFrame =false; 
		mesh_component->specular = glm::vec4(1.0f,1.0f,1.0f,10000.0f);

		sTextureInfo info;
		info.name = "debug_grid.bmp";
		info.strength = 1.0f;
		mesh_component->textures.push_back(info);
		pMazeScene->addEntityToScene(fire_pit);

		fire_pit->components[7] = new cParticleComponent();
		cParticleComponent* particle_component = static_cast<cParticleComponent*>(fire_pit->components[7]);


		cEntity* particle = cEntityBuilder::getInstance()->createEntity(1);
		cRenderMeshComponent* particle_mesh = static_cast<cRenderMeshComponent*>(particle->components[3]);
		static_cast<cTransformComponent*>(particle->components[1])->scale = glm::vec3(0.1f);
		particle_mesh->meshName = "1x1_Star_1_Quad_2_Sided_xyz_n_uv.ply";
		particle_mesh->bIsWireFrame = false;
		particle_mesh->bDontLight = false;

		sTextureInfo tex_info;
		tex_info.name = "fire.bmp";
		tex_info.strength = 1.0f;
		particle_mesh->textures.push_back(tex_info);
		particle_mesh->objColour = glm::vec4(1.0f,0.0,0.0f,1.0f);

		particle_component->particleEntity = particle;

		cParticleComponent::sParticleCreationParams particle_params;
		particle_params.position = glm::vec3(0.0, 0.0, 0.0);
		particle_params.minInitialVelocity = glm::vec3(0.0,0.5f,0.0f);
		particle_params.maxInitialVelocity = glm::vec3(0.2,0.8f,0.2f);
		particle_params.minLifeTime = 0.1;
		particle_params.maxLifeTime = 0.3;
		particle_params.minNumberNewParticles = 0;
		particle_params.maxNumberNewParticles = 2;
		particle_params.minOrientationChangeEuler = glm::vec3(-0.60);
		particle_params.maxOrientationChangeEuler = glm::vec3(0.60);
		particle_params.maxInitialScale = 0.015000000596046449f;
		particle_params.maxScaleChange = 0.001f;
		particle_params.deathTransparencyFadeTimeSeconds = 2.0f;

		cParticleSystem::getInstance()->registerEntity(fire_pit);

		particle_component->init(particle_params);
		particle_component->reset(100,false);
	}
	fires.push_back(fire_2);

	fire_info fire_3;
	{
		cEntity* fire_pit = cEntityBuilder::getInstance()->createEntity(1);
		fire_3.smoke = fire_pit;
		cTransformComponent* transform_component = static_cast<cTransformComponent*>(fire_pit->components[1]);
		transform_component->set_position(glm::vec3(238.3,2.45,16.58));
		transform_component->scale = glm::vec3(0.5f);
		cRenderMeshComponent* mesh_component = static_cast<cRenderMeshComponent*>(fire_pit->components[3]);
		mesh_component->objColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		mesh_component->meshName = "firepit.ply";
		mesh_component->friendlyName = "firepit";
		mesh_component->bIsWireFrame =false; 
		mesh_component->specular = glm::vec4(1.0f,1.0f,1.0f,10000.0f);

		sTextureInfo info;
		info.name = "bark.bmp";
		info.strength = 1.0f;
		mesh_component->textures.push_back(info);
		pMazeScene->addEntityToScene(fire_pit);

		fire_pit->components[7] = new cParticleComponent();
		cParticleComponent* particle_component = static_cast<cParticleComponent*>(fire_pit->components[7]);


		cEntity* particle = cEntityBuilder::getInstance()->createEntity(1);
		cRenderMeshComponent* particle_mesh = static_cast<cRenderMeshComponent*>(particle->components[3]);
		static_cast<cTransformComponent*>(particle->components[1])->scale = glm::vec3(0.1f);
		particle_mesh->meshName = "1x1_Star_1_Quad_2_Sided_xyz_n_uv.ply";
		particle_mesh->bIsWireFrame = false;
		particle_mesh->bDontLight = false;

		sTextureInfo tex_info;
		tex_info.name = "smoke.bmp";
		tex_info.strength = 1.0f;
		particle_mesh->textures.push_back(tex_info);
		particle_mesh->objColour = glm::vec4(0.1f,0.1,0.1f,1.0f);

		particle_component->particleEntity = particle;

		cParticleComponent::sParticleCreationParams particle_params;
		particle_params.position = glm::vec3(0.0, 0.25, 0.0);
		particle_params.minInitialVelocity = glm::vec3(-0.1,0.5f,-0.1f);
		particle_params.maxInitialVelocity = glm::vec3(0.1,1.0f,0.1f);
		particle_params.minLifeTime = 0.8;
		particle_params.maxLifeTime = 2.0;
		particle_params.minNumberNewParticles = 0;
		particle_params.maxNumberNewParticles = 2;
		particle_params.minOrientationChangeEuler = glm::vec3(-0.60);
		particle_params.maxOrientationChangeEuler = glm::vec3(0.60);
		particle_params.maxInitialScale = 0.015000000596046449f;
		particle_params.maxScaleChange = 0.001f;
		particle_params.deathTransparencyFadeTimeSeconds = 2.0f;

		cParticleSystem::getInstance()->registerEntity(fire_pit);

		particle_component->init(particle_params);
		particle_component->reset(100,false);
	}

	{
		cEntity* fire_pit = cEntityBuilder::getInstance()->createEntity(1);
		fire_3.fire = fire_pit;
		cTransformComponent* transform_component = static_cast<cTransformComponent*>(fire_pit->components[1]);
		transform_component->set_position(glm::vec3(238.3,2.45,16.58));
		transform_component->scale = glm::vec3(0.5f);
		cRenderMeshComponent* mesh_component = static_cast<cRenderMeshComponent*>(fire_pit->components[3]);
		mesh_component->objColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		mesh_component->meshName = "firepit.ply";
		mesh_component->friendlyName = "firepit";
		mesh_component->bIsWireFrame =false; 
		mesh_component->specular = glm::vec4(1.0f,1.0f,1.0f,10000.0f);

		sTextureInfo info;
		info.name = "debug_grid.bmp";
		info.strength = 1.0f;
		mesh_component->textures.push_back(info);
		pMazeScene->addEntityToScene(fire_pit);

		fire_pit->components[7] = new cParticleComponent();
		cParticleComponent* particle_component = static_cast<cParticleComponent*>(fire_pit->components[7]);


		cEntity* particle = cEntityBuilder::getInstance()->createEntity(1);
		cRenderMeshComponent* particle_mesh = static_cast<cRenderMeshComponent*>(particle->components[3]);
		static_cast<cTransformComponent*>(particle->components[1])->scale = glm::vec3(0.1f);
		particle_mesh->meshName = "1x1_Star_1_Quad_2_Sided_xyz_n_uv.ply";
		particle_mesh->bIsWireFrame = false;
		particle_mesh->bDontLight = false;

		sTextureInfo tex_info;
		tex_info.name = "fire.bmp";
		tex_info.strength = 1.0f;
		particle_mesh->textures.push_back(tex_info);
		particle_mesh->objColour = glm::vec4(1.0f,0.0,0.0f,1.0f);

		particle_component->particleEntity = particle;

		cParticleComponent::sParticleCreationParams particle_params;
		particle_params.position = glm::vec3(0.0, 0.0, 0.0);
		particle_params.minInitialVelocity = glm::vec3(0.0,0.5f,0.0f);
		particle_params.maxInitialVelocity = glm::vec3(0.2,0.8f,0.2f);
		particle_params.minLifeTime = 0.1;
		particle_params.maxLifeTime = 0.3;
		particle_params.minNumberNewParticles = 0;
		particle_params.maxNumberNewParticles = 2;
		particle_params.minOrientationChangeEuler = glm::vec3(-0.60);
		particle_params.maxOrientationChangeEuler = glm::vec3(0.60);
		particle_params.maxInitialScale = 0.015000000596046449f;
		particle_params.maxScaleChange = 0.001f;
		particle_params.deathTransparencyFadeTimeSeconds = 2.0f;

		cParticleSystem::getInstance()->registerEntity(fire_pit);

		particle_component->init(particle_params);
		particle_component->reset(100,false);
	}
	fires.push_back(fire_3);

	fire_info fire_4;
	{
		cEntity* fire_pit = cEntityBuilder::getInstance()->createEntity(1);
		fire_4.smoke = fire_pit;
		cTransformComponent* transform_component = static_cast<cTransformComponent*>(fire_pit->components[1]);
		transform_component->set_position(glm::vec3(17.3,2.0,-207.5));
		transform_component->scale = glm::vec3(0.5f);
		cRenderMeshComponent* mesh_component = static_cast<cRenderMeshComponent*>(fire_pit->components[3]);
		mesh_component->objColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		mesh_component->meshName = "firepit.ply";
		mesh_component->friendlyName = "firepit";
		mesh_component->bIsWireFrame =false; 
		mesh_component->specular = glm::vec4(1.0f,1.0f,1.0f,10000.0f);

		sTextureInfo info;
		info.name = "bark.bmp";
		info.strength = 1.0f;
		mesh_component->textures.push_back(info);
		pMazeScene->addEntityToScene(fire_pit);

		fire_pit->components[7] = new cParticleComponent();
		cParticleComponent* particle_component = static_cast<cParticleComponent*>(fire_pit->components[7]);


		cEntity* particle = cEntityBuilder::getInstance()->createEntity(1);
		cRenderMeshComponent* particle_mesh = static_cast<cRenderMeshComponent*>(particle->components[3]);
		static_cast<cTransformComponent*>(particle->components[1])->scale = glm::vec3(0.1f);
		particle_mesh->meshName = "1x1_Star_1_Quad_2_Sided_xyz_n_uv.ply";
		particle_mesh->bIsWireFrame = false;
		particle_mesh->bDontLight = false;

		sTextureInfo tex_info;
		tex_info.name = "smoke.bmp";
		tex_info.strength = 1.0f;
		particle_mesh->textures.push_back(tex_info);
		particle_mesh->objColour = glm::vec4(0.1f,0.1,0.1f,1.0f);

		particle_component->particleEntity = particle;

		cParticleComponent::sParticleCreationParams particle_params;
		particle_params.position = glm::vec3(0.0, 0.25, 0.0);
		particle_params.minInitialVelocity = glm::vec3(-0.1,0.5f,-0.1f);
		particle_params.maxInitialVelocity = glm::vec3(0.1,1.0f,0.1f);
		particle_params.minLifeTime = 0.8;
		particle_params.maxLifeTime = 2.0;
		particle_params.minNumberNewParticles = 0;
		particle_params.maxNumberNewParticles = 2;
		particle_params.minOrientationChangeEuler = glm::vec3(-0.60);
		particle_params.maxOrientationChangeEuler = glm::vec3(0.60);
		particle_params.maxInitialScale = 0.015000000596046449f;
		particle_params.maxScaleChange = 0.001f;
		particle_params.deathTransparencyFadeTimeSeconds = 2.0f;

		cParticleSystem::getInstance()->registerEntity(fire_pit);

		particle_component->init(particle_params);
		particle_component->reset(100,false);
	}

	{
		cEntity* fire_pit = cEntityBuilder::getInstance()->createEntity(1);
		fire_4.fire = fire_pit;
		cTransformComponent* transform_component = static_cast<cTransformComponent*>(fire_pit->components[1]);
		transform_component->set_position(glm::vec3(17.3,2.0,-207.5));
		transform_component->scale = glm::vec3(0.5f);
		cRenderMeshComponent* mesh_component = static_cast<cRenderMeshComponent*>(fire_pit->components[3]);
		mesh_component->objColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		mesh_component->meshName = "firepit.ply";
		mesh_component->friendlyName = "firepit";
		mesh_component->bIsWireFrame =false; 
		mesh_component->specular = glm::vec4(1.0f,1.0f,1.0f,10000.0f);

		sTextureInfo info;
		info.name = "debug_grid.bmp";
		info.strength = 1.0f;
		mesh_component->textures.push_back(info);
		pMazeScene->addEntityToScene(fire_pit);

		fire_pit->components[7] = new cParticleComponent();
		cParticleComponent* particle_component = static_cast<cParticleComponent*>(fire_pit->components[7]);


		cEntity* particle = cEntityBuilder::getInstance()->createEntity(1);
		cRenderMeshComponent* particle_mesh = static_cast<cRenderMeshComponent*>(particle->components[3]);
		static_cast<cTransformComponent*>(particle->components[1])->scale = glm::vec3(0.1f);
		particle_mesh->meshName = "1x1_Star_1_Quad_2_Sided_xyz_n_uv.ply";
		particle_mesh->bIsWireFrame = false;
		particle_mesh->bDontLight = false;

		sTextureInfo tex_info;
		tex_info.name = "fire.bmp";
		tex_info.strength = 1.0f;
		particle_mesh->textures.push_back(tex_info);
		particle_mesh->objColour = glm::vec4(1.0f,0.0,0.0f,1.0f);

		particle_component->particleEntity = particle;

		cParticleComponent::sParticleCreationParams particle_params;
		particle_params.position = glm::vec3(0.0, 0.0, 0.0);
		particle_params.minInitialVelocity = glm::vec3(0.0,0.5f,0.0f);
		particle_params.maxInitialVelocity = glm::vec3(0.2,0.8f,0.2f);
		particle_params.minLifeTime = 0.1;
		particle_params.maxLifeTime = 0.3;
		particle_params.minNumberNewParticles = 0;
		particle_params.maxNumberNewParticles = 2;
		particle_params.minOrientationChangeEuler = glm::vec3(-0.60);
		particle_params.maxOrientationChangeEuler = glm::vec3(0.60);
		particle_params.maxInitialScale = 0.015000000596046449f;
		particle_params.maxScaleChange = 0.001f;
		particle_params.deathTransparencyFadeTimeSeconds = 2.0f;

		cParticleSystem::getInstance()->registerEntity(fire_pit);

		particle_component->init(particle_params);
		particle_component->reset(100,false);
	}
	fires.push_back(fire_4);

	fire_info fire_5;
	{
		cEntity* fire_pit = cEntityBuilder::getInstance()->createEntity(1);
		fire_5.smoke = fire_pit;
		cTransformComponent* transform_component = static_cast<cTransformComponent*>(fire_pit->components[1]);
		transform_component->set_position(glm::vec3(102.93,19.06,94.37));
		transform_component->scale = glm::vec3(0.5f);
		cRenderMeshComponent* mesh_component = static_cast<cRenderMeshComponent*>(fire_pit->components[3]);
		mesh_component->objColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		mesh_component->meshName = "firepit.ply";
		mesh_component->friendlyName = "firepit";
		mesh_component->bIsWireFrame =false; 
		mesh_component->specular = glm::vec4(1.0f,1.0f,1.0f,10000.0f);

		sTextureInfo info;
		info.name = "bark.bmp";
		info.strength = 1.0f;
		mesh_component->textures.push_back(info);
		pMazeScene->addEntityToScene(fire_pit);

		fire_pit->components[7] = new cParticleComponent();
		cParticleComponent* particle_component = static_cast<cParticleComponent*>(fire_pit->components[7]);


		cEntity* particle = cEntityBuilder::getInstance()->createEntity(1);
		cRenderMeshComponent* particle_mesh = static_cast<cRenderMeshComponent*>(particle->components[3]);
		static_cast<cTransformComponent*>(particle->components[1])->scale = glm::vec3(0.1f);
		particle_mesh->meshName = "1x1_Star_1_Quad_2_Sided_xyz_n_uv.ply";
		particle_mesh->bIsWireFrame = false;
		particle_mesh->bDontLight = false;

		sTextureInfo tex_info;
		tex_info.name = "smoke.bmp";
		tex_info.strength = 1.0f;
		particle_mesh->textures.push_back(tex_info);
		particle_mesh->objColour = glm::vec4(0.1f,0.1,0.1f,1.0f);

		particle_component->particleEntity = particle;

		cParticleComponent::sParticleCreationParams particle_params;
		particle_params.position = glm::vec3(0.0, 0.25, 0.0);
		particle_params.minInitialVelocity = glm::vec3(-0.1,0.5f,-0.1f);
		particle_params.maxInitialVelocity = glm::vec3(0.1,1.0f,0.1f);
		particle_params.minLifeTime = 0.8;
		particle_params.maxLifeTime = 2.0;
		particle_params.minNumberNewParticles = 0;
		particle_params.maxNumberNewParticles = 2;
		particle_params.minOrientationChangeEuler = glm::vec3(-0.60);
		particle_params.maxOrientationChangeEuler = glm::vec3(0.60);
		particle_params.maxInitialScale = 0.015000000596046449f;
		particle_params.maxScaleChange = 0.001f;
		particle_params.deathTransparencyFadeTimeSeconds = 2.0f;

		cParticleSystem::getInstance()->registerEntity(fire_pit);

		particle_component->init(particle_params);
		particle_component->reset(100,false);
	}

	{
		cEntity* fire_pit = cEntityBuilder::getInstance()->createEntity(1);
		fire_5.fire = fire_pit;
		cTransformComponent* transform_component = static_cast<cTransformComponent*>(fire_pit->components[1]);
		transform_component->set_position(glm::vec3(102.93,19.06,94.37));
		transform_component->scale = glm::vec3(0.5f);
		cRenderMeshComponent* mesh_component = static_cast<cRenderMeshComponent*>(fire_pit->components[3]);
		mesh_component->objColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		mesh_component->meshName = "firepit.ply";
		mesh_component->friendlyName = "firepit";
		mesh_component->bIsWireFrame =false; 
		mesh_component->specular = glm::vec4(1.0f,1.0f,1.0f,10000.0f);

		sTextureInfo info;
		info.name = "debug_grid.bmp";
		info.strength = 1.0f;
		mesh_component->textures.push_back(info);
		pMazeScene->addEntityToScene(fire_pit);

		fire_pit->components[7] = new cParticleComponent();
		cParticleComponent* particle_component = static_cast<cParticleComponent*>(fire_pit->components[7]);


		cEntity* particle = cEntityBuilder::getInstance()->createEntity(1);
		cRenderMeshComponent* particle_mesh = static_cast<cRenderMeshComponent*>(particle->components[3]);
		static_cast<cTransformComponent*>(particle->components[1])->scale = glm::vec3(0.1f);
		particle_mesh->meshName = "1x1_Star_1_Quad_2_Sided_xyz_n_uv.ply";
		particle_mesh->bIsWireFrame = false;
		particle_mesh->bDontLight = false;

		sTextureInfo tex_info;
		tex_info.name = "fire.bmp";
		tex_info.strength = 1.0f;
		particle_mesh->textures.push_back(tex_info);
		particle_mesh->objColour = glm::vec4(1.0f,0.0,0.0f,1.0f);

		particle_component->particleEntity = particle;

		cParticleComponent::sParticleCreationParams particle_params;
		particle_params.position = glm::vec3(0.0, 0.0, 0.0);
		particle_params.minInitialVelocity = glm::vec3(0.0,0.5f,0.0f);
		particle_params.maxInitialVelocity = glm::vec3(0.2,0.8f,0.2f);
		particle_params.minLifeTime = 0.1;
		particle_params.maxLifeTime = 0.3;
		particle_params.minNumberNewParticles = 0;
		particle_params.maxNumberNewParticles = 2;
		particle_params.minOrientationChangeEuler = glm::vec3(-0.60);
		particle_params.maxOrientationChangeEuler = glm::vec3(0.60);
		particle_params.maxInitialScale = 0.015000000596046449f;
		particle_params.maxScaleChange = 0.001f;
		particle_params.deathTransparencyFadeTimeSeconds = 2.0f;

		cParticleSystem::getInstance()->registerEntity(fire_pit);

		particle_component->init(particle_params);
		particle_component->reset(100,false);
	}
	fires.push_back(fire_5);

	//hint tvs
	{
		cEntity* hint_tv = cEntityBuilder::getInstance()->createEntity(1);
		cTransformComponent* hint_tv_transform = static_cast<cTransformComponent*>(hint_tv->components[1]);
		hint_tv_transform->set_position(glm::vec3(-127.45,2.25,-11.52));

		cRenderMeshComponent* hint_tv_mesh = static_cast<cRenderMeshComponent*>(hint_tv->components[3]);
		hint_tv_mesh->meshName = "RetroTV.edited.bodyonly.ply";

		static_cast<cRenderMeshComponent*>(hint_tv->components[3])->objColour = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		static_cast<cRenderMeshComponent*>(hint_tv->components[3])->meshName = "RetroTV.edited.bodyonly.ply";
		static_cast<cRenderMeshComponent*>(hint_tv->components[3])->bIsVisible = true;
		static_cast<cRenderMeshComponent*>(hint_tv->components[3])->bIsWireFrame = false;
		static_cast<cTransformComponent*>(hint_tv->components[1])->setOrientationEulerAngles(glm::vec3(0.0f,90.0f,0.0f), true);
		static_cast<cTransformComponent*>(hint_tv->components[1])->scale = glm::vec3(0.125f);
	}

	{
		cEntity* hint_tv = cEntityBuilder::getInstance()->createEntity(1);
		cTransformComponent* hint_tv_transform = static_cast<cTransformComponent*>(hint_tv->components[1]);
		hint_tv_transform->set_position(glm::vec3(-127.45,2.25,-10.52));

		cRenderMeshComponent* hint_tv_mesh = static_cast<cRenderMeshComponent*>(hint_tv->components[3]);
		hint_tv_mesh->meshName = "RetroTV.edited.bodyonly.ply";

		static_cast<cRenderMeshComponent*>(hint_tv->components[3])->objColour = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		static_cast<cRenderMeshComponent*>(hint_tv->components[3])->meshName = "RetroTV.edited.bodyonly.ply";
		static_cast<cRenderMeshComponent*>(hint_tv->components[3])->bIsVisible = true;
		static_cast<cRenderMeshComponent*>(hint_tv->components[3])->bIsWireFrame = false;
		static_cast<cTransformComponent*>(hint_tv->components[1])->setOrientationEulerAngles(glm::vec3(0.0f,90.0f,0.0f), true);
		static_cast<cTransformComponent*>(hint_tv->components[1])->scale = glm::vec3(0.125f);
	}

	//tv fbos
	cEntity* tv1 = cEntityBuilder::getInstance()->createEntity(3);
	static_cast<cRenderMeshComponent*>(tv1->components[3])->friendlyName = "tv1";
	static_cast<cRenderMeshComponent*>(tv1->components[3])->render_target_overlay = true;
	static_cast<cFrameBufferComponent*>(tv1->components[9])->inititFrameBuffer(0);
	static_cast<cTransformComponent*>(tv1->components[1])->setOrientationEulerAngles(glm::vec3(180.0, 0.0f, 90.0), true);
	static_cast<cTransformComponent*>(tv1->components[1])->set_position(glm::vec3(-127.35,2.45,-11.45));
	static_cast<cTransformComponent*>(tv1->components[1])->scale = glm::vec3(0.12);
	fbos.push_back(tv1);
	pMazeScene->addEntityToScene(tv1);

	cEntity* tv2 = cEntityBuilder::getInstance()->createEntity(3);
	static_cast<cRenderMeshComponent*>(tv2->components[3])->friendlyName = "tv2";
	static_cast<cRenderMeshComponent*>(tv2->components[3])->render_target_overlay = true;
	static_cast<cFrameBufferComponent*>(tv2->components[9])->inititFrameBuffer(0);
	static_cast<cTransformComponent*>(tv2->components[1])->setOrientationEulerAngles(glm::vec3(180.0, 0.0f, 90.0), true);
	static_cast<cTransformComponent*>(tv2->components[1])->set_position(glm::vec3(-127.35,2.45,-10.45));
	static_cast<cTransformComponent*>(tv2->components[1])->scale = glm::vec3(0.12);
	fbos.push_back(tv2);
	pMazeScene->addEntityToScene(tv2);

	cSoundManager::sSoundInfo* background_music = new cSoundManager::sSoundInfo();
	background_music->fileName = "background.wav";

	cSoundManager::getInstance()->setBasePath("./assets/sounds/");
	cSoundManager::getInstance()->loadStream(background_music);
	cSoundManager::getInstance()->playSound(background_music);
	
}