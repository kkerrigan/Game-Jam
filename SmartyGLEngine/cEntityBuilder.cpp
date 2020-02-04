#include "cEntityBuilder.h"
#include "cComponentFactory.h"

#include "cEulerMovementSystem.h"
#include "cMeshRenderSystem.h"
#include "cDebugRenderSystem.h"
#include "cColliderSystem.h"

#include "cRenderMeshComponent.h"
#include "cTransformComponent.h"
#include "cEulerMovementComponent.h"
#include "cDebugRenderComponent.h"
#include "cCollider.h"

#include "physicsShapes.h"
#include "cShaderManager.h"
#include "cAttachedLightsComponent.h"
#include "cLightManager.h"
#include "cSceneManager.h"
#include "c3dSoundComponent.h"
#include "cSoundManager.h"
#include "cParticleSystem.h"
#include "cParticleComponent.h"
#include "cFrameBufferComponent.h"
#include "cSkinnedMeshComponent.h"
#include "cInstancedMeshComponent.h"
#include "cPhysicsComponent.h"
#include "cEngine.h"


bool load_animations_threaded = true;

bool assimpToVAOConverter(cSimpleAssimpSkinnedMesh* pTheAssimpSM);

void ballDebugRendering(cEntity* entity);

cEntityBuilder::cEntityBuilder() {
}


cEntityBuilder::~cEntityBuilder() {
	for (cEntity* entity : this->allocatedEntites) {
		delete entity;
	}
}

cEntityBuilder* cEntityBuilder::getInstance() {
	static cEntityBuilder instance;

	return &instance;
}

cEntity* cEntityBuilder::createEntity(int entityType) {
	cEntity* result = nullptr;

	switch (entityType) {

		//allowed entity types
	case 0: // use this one or pain will come
	case 1: // Sky box object
	case 2: // fbo entity
	case 3:
	case 4: //skin mesh (man)
	case 5: //instanced mesh (static blocks)
	case 6: //resource gatherer
	case 7: //skin mesh (boy)
	case 8: //particle system
		result = new cEntity(entityType);
		break;
	}

	if (result != nullptr) {
		// add the components required
		addRequiredComponents(result, entityType);

		this->allocatedEntites.push_back(result);
	}

	return result;
}

cEntity* cEntityBuilder::createEntity(int entityType, glm::vec3 position) {
	cEntity* result = nullptr;

	result = this->createEntity(entityType);

	this->setEntityPosition(result, position);

	return result;
}

void cEntityBuilder::setEntityPosition(cEntity* entity, glm::vec3 position) {

	cTransformComponent* transformComponent = (cTransformComponent*)entity->components[1];
	transformComponent->set_position(position);
}

cEntity* cEntityBuilder::createEntityRandomPosColor(int entityType) {
	cEntity* result = this->createEntity(entityType);

	// make sure we have the compoents required
	if ((result->componentBitField & 5) != 5) {
		return result;
	}

	cTransformComponent* pTransformComponent = static_cast<cTransformComponent*>(result->components[1]);
	pTransformComponent->set_position(glm::vec3(this->getRandInRange<float>(-75.0f, 75.0f), this->getRandInRange<float>(-75.0f, 75.0f), this->getRandInRange<float>(-75.0f, 75.0f)));

	cRenderMeshComponent* pRenderMeshComponent = static_cast<cRenderMeshComponent*>(result->components[3]);
	pRenderMeshComponent->objColour = glm::vec4(this->getRandBetween0and1<float>(), this->getRandBetween0and1<float>(), this->getRandBetween0and1<float>(), 1.0f);
	return result;
}

void cEntityBuilder::addRequiredComponents(cEntity* entity, int entityType) {

	cComponentFactory* pComponentFactory = cComponentFactory::getInstance();

	switch (entityType)
	{

	case 0: //new entity setup
	{
		entity->components[8] = pComponentFactory->createComponent(8);

		entity->components[3] = pComponentFactory->createComponent(3);
		{
			cRenderMeshComponent* meshComp = static_cast<cRenderMeshComponent*>(entity->components[3]);
			meshComp->meshName = "Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply";
			meshComp->bIsVisible = true;
			meshComp->bIsWireFrame = true;
			meshComp->objColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			meshComp->bUseVertexColour = false;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
		}

		// register the entity with the component systems
		cMeshRenderSystem::getInstance()->registerEntity(entity);
	}
	break;

	case 1: //old entity setup
	{
		entity->components[1] = pComponentFactory->createComponent(1);

		cTransformComponent* pTransformComponent = static_cast<cTransformComponent*>(entity->components[1]);
		pTransformComponent->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
		pTransformComponent->scale = glm::vec3(5000.0f);

		entity->components[3] = pComponentFactory->createComponent(3);
		{
			cRenderMeshComponent* meshComp = static_cast<cRenderMeshComponent*>(entity->components[3]);
			meshComp->meshName = "Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply";
			meshComp->friendlyName = "SkyBoxObject";
			meshComp->bIsVisible = true;
			meshComp->objColour = glm::vec4(1.0f, 105.0f / 255.0f, 180.0f / 255.0f, 1.0f);
			meshComp->bUseVertexColour = false;
			meshComp->bDontLight = false;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
		}

		// register the entity with the component systems
		cMeshRenderSystem::getInstance()->registerEntity(entity);

		//entity->components[11] = pComponentFactory->createComponent(11);

	}
	break;


	case 2:
		entity->components[1] = pComponentFactory->createComponent(1);
		static_cast<cTransformComponent*>(entity->components[1])->scale = glm::vec3(10.0f);
		static_cast<cTransformComponent*>(entity->components[1])->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
		static_cast<cTransformComponent*>(entity->components[1])->adjOrientationEulerAngles(270.0f, 0.0f, 90.0f, true);

		entity->components[9] = pComponentFactory->createComponent(9);

		entity->components[3] = pComponentFactory->createComponent(3);
		{
			cRenderMeshComponent* meshComp = static_cast<cRenderMeshComponent*>(entity->components[3]);
			meshComp->meshName = "1x1_2Tri_Quad_2_Sided_xyz_n_uv.ply";
			meshComp->bIsVisible = true;
			meshComp->bIsWireFrame = false;
			meshComp->objColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			meshComp->bUseVertexColour = false;
			meshComp->bDontLight = true;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
			meshComp->use_offscreen_texture = true;
		}

		// register the entity with the component systems
		cMeshRenderSystem::getInstance()->registerEntity(entity);
		break;


	case 3:
		entity->components[1] = pComponentFactory->createComponent(1);
		static_cast<cTransformComponent*>(entity->components[1])->scale = glm::vec3(20.0f);
		static_cast<cTransformComponent*>(entity->components[1])->set_position(glm::vec3(10.0f, 85.0f, -12.0f));
		static_cast<cTransformComponent*>(entity->components[1])->adjOrientationEulerAngles(90.0f, 0.0f, 90.0f, true);

		entity->components[9] = pComponentFactory->createComponent(9);

		entity->components[3] = pComponentFactory->createComponent(3);
		{
			cRenderMeshComponent* meshComp = static_cast<cRenderMeshComponent*>(entity->components[3]);
			meshComp->meshName = "1x1_2Tri_Quad_2_Sided_xyz_n_uv.ply";
			meshComp->bIsVisible = true;
			meshComp->bIsWireFrame = false;
			meshComp->objColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			meshComp->bUseVertexColour = false;
			meshComp->bDontLight = true;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
			meshComp->use_offscreen_texture = true;
			meshComp->is_in_world_fbo = true;
		}

		break;


	case 4:
		entity->components[8] = pComponentFactory->createComponent(8);
		{
			cPhysicsComponent* physics_component = static_cast<cPhysicsComponent*>(entity->components[8]);
			auto physics_factory = cEngine::getInstance()->get_physics_factory();
			physics::defs::sRigidBodyDef def;
			def.height = 0.375f;
			def.radius = 0.25f;
			def.mass = 1.0f;
			def.shape_type = physics::interfaces::iShape::shape_types::capsule;
			def.scale = glm::vec3(0.009);
			auto body = physics_factory->create_rigid_body(def);

			body->set_position(glm::vec3(-123.4,3.3,-8.23));
			physics_component->configure_component(body);

			cEngine::getInstance()->get_world()->register_rigid_body(body);
		}

		/*entity->componentBitField |= 1;
		entity->components[1] = pComponentFactory->createComponent(1);
		{
			cTransformComponent* transformComponent = static_cast<cTransformComponent*>(entity->components[1]);
			transformComponent->set_position(glm::vec3(80.0f, 0.0f, 0.0f));
			transformComponent->setOrientationEulerAngles(glm::vec3(0.0f, 0.0f, 0.0f), true);
			transformComponent->scale = glm::vec3(0.09f);
		}*/

		entity->componentBitField |= 2;
		entity->components[2] = pComponentFactory->createComponent(2);

		entity->componentBitField |= 4;
		entity->components[3] = pComponentFactory->createComponent(3);
		{
			cRenderMeshComponent* meshComp = (cRenderMeshComponent*)entity->components[3];
			meshComp->meshName = "RPG-Character";
			meshComp->friendlyName = "man";
			meshComp->bIsVisible = true;
			meshComp->objColour = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
			meshComp->bUseVertexColour = false;
			meshComp->bDontLight = false;
			meshComp->bIsWireFrame = false;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
			meshComp->specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

			sTextureInfo texture;
			texture.name = "man.bmp";
			texture.strength = 1.0f;
			meshComp->textures.push_back(texture);

			// Register with the Mesh system
			cMeshRenderSystem::getInstance()->registerEntity(entity);
		}

		entity->components[10] = pComponentFactory->createComponent(10);
		{
			//Animation Midterm - Question # 3
			cSkinnedMeshComponent* skinMeshComp = static_cast<cSkinnedMeshComponent*>(entity->components[10]);
			skinMeshComp->meshName = "assets/modelsFBX/man.FBX"; // TODO: add base path

			if (!skinMeshComp->pSkinnedMesh->loadMeshFromFile(skinMeshComp->meshName)) {
				std::cout << "ERROR: Problem loading the skinned mesh!" << std::endl;
			}

			if (load_animations_threaded)
			{
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Idle", "assets/modelsFBX/man_idle.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Walk", "assets/modelsFBX/man_walk.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Walk Back", "assets/modelsFBX/man_walk_back.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Run", "assets/modelsFBX/man_run.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Run Back", "assets/modelsFBX/man_run_back.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Jump", "assets/modelsFBX/man_jump.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Jump Walk", "assets/modelsFBX/man_jump_walk.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Jump Run", "assets/modelsFBX/man_jump_run.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Strafe Left", "assets/modelsFBX/man_strafe_left.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Strafe Right", "assets/modelsFBX/man_strafe_right.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Dance", "assets/modelsFBX/man_twerk.fbx");
			}
			else
			{
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Idle", "assets/modelsFBX/man_idle.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Walk", "assets/modelsFBX/man_walk.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Walk Back", "assets/modelsFBX/man_walk_back.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Run", "assets/modelsFBX/man_run.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Run Back", "assets/modelsFBX/man_run_back.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Jump", "assets/modelsFBX/man_jump.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Jump Walk", "assets/modelsFBX/man_jump_walk.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Jump Run", "assets/modelsFBX/man_jump_run.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Strafe Left", "assets/modelsFBX/man_strafe_left.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Strafe Right", "assets/modelsFBX/man_strafe_right.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Dance", "assets/modelsFBX/man_twerk.fbx");
			}

			skinMeshComp->pSkinnedMesh->friendlyName = "RPG-Chacter";

			// NOTE: Not sure if next line is necessary 
			skinMeshComp->pMesh = skinMeshComp->pSkinnedMesh->createMeshObjectFromCurrentModel();
			if (skinMeshComp->pMesh) {
				std::cout << "RPG-Character was loaded" << std::endl;
			}
			else {
				std::cout << "ERROR: Didn't load the RPG-Character" << std::endl;
			}

			// Convert assimp model to VAO
			if (!assimpToVAOConverter(skinMeshComp->pSkinnedMesh)) {
				std::cout << "ERROR: Didn't copy RPG-Character to VAO Format" << std::endl;
			}

			skinMeshComp->pAniState = new cAnimationState();
			skinMeshComp->pAniState->defaultAnimation.name = "Idle";
		}
		break;

	case 5: //instanced
	{
		entity->components[1] = pComponentFactory->createComponent(1);
		entity->components[12] = pComponentFactory->createComponent(12);
		cInstancedMeshComponent* instanced_mesh = static_cast<cInstancedMeshComponent*>(entity->components[12]);

		entity->components[3] = pComponentFactory->createComponent(3);
		{
			cRenderMeshComponent* meshComp = static_cast<cRenderMeshComponent*>(entity->components[3]);
			meshComp->meshName = "cube_flat_shaded_xyz_n_uv.ply";
			meshComp->friendlyName = "instanced_mesh";
			meshComp->bIsVisible = true;
			meshComp->objColour = glm::vec4(0.0f, 0.0, 0.0, 1.0f);
			meshComp->bUseVertexColour = false;
			meshComp->bIsWireFrame = false;
			meshComp->bDontLight = false;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");

			sTextureInfo tex_info;
			tex_info.name = "debug_grid.bmp";
			tex_info.strength = 1.0f;

			meshComp->textures.push_back(tex_info);
		}

		// register the entity with the component systems
		cMeshRenderSystem::getInstance()->registerEntity(entity);
		break;
	}
	case 6: //gatherer
	{
		entity->components[1] = pComponentFactory->createComponent(1);

		cTransformComponent* pTransformComponent = static_cast<cTransformComponent*>(entity->components[1]);
		pTransformComponent->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
		pTransformComponent->scale = glm::vec3(5000.0f);

		entity->components[3] = pComponentFactory->createComponent(3);
		{
			cRenderMeshComponent* meshComp = static_cast<cRenderMeshComponent*>(entity->components[3]);
			meshComp->meshName = "dalek2005_xyz_uv_res_2.ply";
			meshComp->friendlyName = "dalek";
			meshComp->bIsVisible = true;
			meshComp->objColour = glm::vec4(1.0f, 105.0f / 255.0f, 180.0f / 255.0f, 1.0f);
			meshComp->bUseVertexColour = false;
			meshComp->bDontLight = false;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
		}

		// register the entity with the component systems
		cMeshRenderSystem::getInstance()->registerEntity(entity);

		//entity->components[11] = pComponentFactory->createComponent(11);

	}
	break;

	case 7:
		entity->componentBitField |= 1;
		entity->components[1] = pComponentFactory->createComponent(1);
		{
			cTransformComponent* transformComponent = static_cast<cTransformComponent*>(entity->components[1]);
			transformComponent->set_position(glm::vec3(20.0f, 0.0f, 0.0f));
			transformComponent->setOrientationEulerAngles(glm::vec3(0.0f, 0.0f, 0.0f), true);
			transformComponent->scale = glm::vec3(0.025f);
		}

		entity->componentBitField |= 2;
		entity->components[2] = pComponentFactory->createComponent(2);

		entity->componentBitField |= 4;
		entity->components[3] = pComponentFactory->createComponent(3);
		{
			cRenderMeshComponent* meshComp = (cRenderMeshComponent*)entity->components[3];
			meshComp->meshName = "RPG-Character";
			meshComp->friendlyName = "Boy";
			meshComp->bIsVisible = true;
			meshComp->objColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			meshComp->bUseVertexColour = false;
			meshComp->bDontLight = false;
			meshComp->bIsWireFrame = false;
			meshComp->shaderId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");
			meshComp->specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

			sTextureInfo texture;
			texture.name = "man.bmp";
			texture.strength = 1.0f;
			meshComp->textures.push_back(texture);

			// Register with the Mesh system
			cMeshRenderSystem::getInstance()->registerEntity(entity);
		}

		entity->components[10] = pComponentFactory->createComponent(10);
		{
			//Animation Midterm - Question # 3
			cSkinnedMeshComponent* skinMeshComp = static_cast<cSkinnedMeshComponent*>(entity->components[10]);
			skinMeshComp->meshName = "assets/modelsFBX/boy.FBX"; // TODO: add base path

			if (!skinMeshComp->pSkinnedMesh->loadMeshFromFile(skinMeshComp->meshName)) {
				std::cout << "ERROR: Problem loading the skinned mesh!" << std::endl;
			}

			if (load_animations_threaded)
			{
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Idle", "assets/modelsFBX/boy_idle.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Walk", "assets/modelsFBX/boy_walk.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Walk Back", "assets/modelsFBX/boy_walk_back.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Run", "assets/modelsFBX/boy_run.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Run Back", "assets/modelsFBX/boy_run_back.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Jump", "assets/modelsFBX/boy_jump.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Jump Walk", "assets/modelsFBX/man_jump_walk.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Jump Run", "assets/modelsFBX/boy_jump_run.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Strafe Left", "assets/modelsFBX/boy_strafe_left.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Strafe Right", "assets/modelsFBX/boy_strafe_right.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation_Threaded("Dance", "assets/modelsFBX/boy_dance.fbx");
			}
			else
			{
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Idle", "assets/modelsFBX/boy_idle.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Walk", "assets/modelsFBX/boy_walk.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Walk Back", "assets/modelsFBX/boy_walk_back.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Run", "assets/modelsFBX/boy_run.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Run Back", "assets/modelsFBX/boy_run_back.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Jump", "assets/modelsFBX/boy_jump.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Jump Walk", "assets/modelsFBX/man_jump_walk.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Jump Run", "assets/modelsFBX/boy_jump_run.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Strafe Left", "assets/modelsFBX/boy_strafe_left.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Strafe Right", "assets/modelsFBX/boy_strafe_right.fbx");
				skinMeshComp->pSkinnedMesh->loadMeshAnimation("Dance", "assets/modelsFBX/boy_dance.fbx");
			}


			skinMeshComp->pSkinnedMesh->friendlyName = "RPG-Chacter";

			// NOTE: Not sure if next line is necessary 
			skinMeshComp->pMesh = skinMeshComp->pSkinnedMesh->createMeshObjectFromCurrentModel();
			if (skinMeshComp->pMesh) {
				std::cout << "RPG-Character was loaded" << std::endl;
			}
			else {
				std::cout << "ERROR: Didn't load the RPG-Character" << std::endl;
			}

			// Convert assimp model to VAO
			if (!assimpToVAOConverter(skinMeshComp->pSkinnedMesh)) {
				std::cout << "ERROR: Didn't copy RPG-Character to VAO Format" << std::endl;
			}

			skinMeshComp->pAniState = new cAnimationState();
			skinMeshComp->pAniState->defaultAnimation.name = "Idle";
		}
		break;

	case 8:
		{
			
		}
		break;
	}
}

template<typename T>
T cEntityBuilder::getRandBetween0and1(void)
{
	return (T)((double)rand() / (RAND_MAX));
}

template <class T>
T cEntityBuilder::getRandInRange(T min, T max)
{
	double value =
		min + static_cast <double> (rand())
		/ (static_cast <double> (RAND_MAX / (static_cast<double>(max - min))));
	return static_cast<T>(value);
}