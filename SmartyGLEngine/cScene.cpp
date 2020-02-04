#include "cScene.h"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <rapidjson/prettywriter.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/istreamwrapper.h>

#include "cLightHelper.h"
#include "cShaderManager.h"
#include "cParticleComponent.h"
#include "cEulerMovementComponent.h"
#include "cTransformComponent.h"
#include "cRenderMeshComponent.h"
#include "cMeshRenderSystem.h"
#include "physics/defs/sRigidBodyDef.h"
#include "cPhysicsComponent.h"
#include "cEngine.h"
#include "cSceneManager.h"
#include "cMazeHelpers.h"

cScene::cScene() : cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)), selectedEntity(0), selectedLight(0), selectedChannelGroup(0) {

	this->bIsLightDebug = false;
	this->bIsRenderDebug = false;
	this->time_of_day = 0.0f;
	this->skybox_weights[0].x = 1.0f;
	return;
}

cScene::~cScene() {}

//Gems - Question #2 - find a corridor to put the dalek in
glm::vec3 cScene::find_corridor_position()
{
	float x = rand() % 400 - 200;
	float z = rand() % 400 - 200;

	glm::vec3 position(x, 0.0f, z);

	//check if it is a corridor
	unsigned long long cube_id;
	cMazeHelpers::find_closest_cube(theMM.maze, position, &cube_id);

	std::map<unsigned long long, cMazeMaker::maze_cube_info>::iterator it = this->maze_info.find(cube_id);

	if (it == this->maze_info.end())
	{
		//make it float in the origin if something went wrong (this would be a position outside the loaded maze)
		return glm::vec3(0.0f, 5.0f, 0.0f);
	}
	else
	{
		if (!it->second.is_wall)				//it's a good spot to put the dalek
		{
			return it->second.center;
		}
		else
		{
			return this->find_corridor_position(); //this spot is hot garbage(a wall), better find another one (recursively)
		}
	}
}

void cScene::setSceneName(std::string name) {
	this->sceneName = name;
}

void cScene::updateWindowTitle(GLFWwindow* window) {

	cEntity* curObject = this->getSelectedEntity();
	cRenderMeshComponent* curMesh = static_cast<cRenderMeshComponent*>(curObject->components[3]);
	cTransformComponent* curTransform = static_cast<cTransformComponent*>(curObject->components[1]);

	sLight* curLight = this->getSelectedLight();

	// Set the window title
	std::stringstream ssTitle;
	ssTitle << "Camera (x,y,z) "
		<< " Eye "
		<< this->flyCamera.eye.x << ", " << this->flyCamera.eye.y << ", " << this->flyCamera.eye.z
		<< " Selected Model[" << curMesh->friendlyName << "] position: "
		<< curTransform->get_position().x << ", " << curTransform->get_position().y << ", " << curTransform->get_position().z
		<< " Selected Light[" << this->selectedLight << "] position: "
		<< curLight->position.x << ", " << curLight->position.y << ", " << curLight->position.z
		<< " Const: " << curLight->atten.x << " Linear: " << curLight->atten.y << " Quad: " << std::setprecision(9) << std::showpoint << std::fixed << curLight->atten.z;

	glfwSetWindowTitle(window, ssTitle.str().c_str());

	return;
}

bool cScene::getIsScenePaused() {
	return this->isScenePaused;
}

void cScene::setIsScenePaused(bool val) {
	this->isScenePaused = val;
}

void cScene::setCameraPosition(glm::vec3 pos) {
	this->cameraPos = pos;
}

void cScene::setCameraLookAt(glm::vec3 lookAt) {
	this->cameraLookAt = lookAt;
}

//Graphics Midterm - Question #1
void cScene::build_maze()
{
	theMM.GenerateMaze(100, 100);

	float offsetX = -200.0f;
	float offsetY = -200.0f;

	for (unsigned int a = 0; a < this->theMM.maze.size(); a++)
	{
		for (unsigned int b = 0; b < this->theMM.maze[a].size(); b++)
		{
			if (this->theMM.maze[a][b][0].is_wall)
			{
				cEntity* new_entity = cEntityBuilder::getInstance()->createEntity(0);
				physics::defs::sRigidBodyDef def;
				def.shape_type = physics::interfaces::iShape::shape_types::sphere;
				def.radius = 1.0f;
				def.position = glm::vec3(a * 4.0f + offsetX, 0.0f, b*4.0f + offsetY);
				this->theMM.maze[a][b][0].center = def.position;

				cMazeHelpers::find_closest_cube(this->theMM.maze, def.position, &this->theMM.maze[a][b][0].cube_id);

				def.mass = 1.0f;
				def.scale = glm::vec3(2.0f);
				cEngine* engine = cEngine::getInstance();
				physics::interfaces::iPhysicsFactory* factory = engine->get_physics_factory();

				cPhysicsComponent* physics = static_cast<cPhysicsComponent*>(new_entity->components[8]);

				physics::interfaces::iRigidBody* body = factory->create_rigid_body(def);
				physics->configure_component(body);

				cRenderMeshComponent* mesh = static_cast<cRenderMeshComponent*>(new_entity->components[3]);
				mesh->meshName = "cube_flat_shaded_xyz_n_uv.ply";
				mesh->bIsWireFrame = false;
				mesh->bDontLight = false;
				mesh->objColour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

				mesh->specular = glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f);

				sTextureInfo textureInfo;
				textureInfo.name = "rocks.bmp";
				textureInfo.strength = 1.0f;

				mesh->textures.push_back(textureInfo);
				this->addEntityToScene(new_entity);

				cMeshRenderSystem::getInstance()->registerEntity(new_entity);

				this->theMM.maze[a][b][0].entity = new_entity;
				maze_info[this->theMM.maze[a][b][0].cube_id] = this->theMM.maze[a][b][0];

			}
			else
			{
				cEntity* new_entity = cEntityBuilder::getInstance()->createEntity(0);
				physics::defs::sRigidBodyDef def;
				def.shape_type = physics::interfaces::iShape::shape_types::sphere;
				def.radius = 1.0f;
				def.position = glm::vec3(a * 4.0f + offsetX, 0.0f, b*4.0f + offsetY);

				cMazeHelpers::find_closest_cube(this->theMM.maze, def.position, &this->theMM.maze[a][b][0].cube_id);

				def.mass = 1.0f;
				def.scale = glm::vec3(2.0f);
				cEngine* engine = cEngine::getInstance();
				physics::interfaces::iPhysicsFactory* factory = engine->get_physics_factory();

				cPhysicsComponent* physics = static_cast<cPhysicsComponent*>(new_entity->components[8]);

				physics::interfaces::iRigidBody* body = factory->create_rigid_body(def);
				physics->configure_component(body);

				cRenderMeshComponent* mesh = static_cast<cRenderMeshComponent*>(new_entity->components[3]);
				mesh->meshName = "cube_flat_shaded_xyz_n_uv.ply";
				mesh->bIsWireFrame = false;
				mesh->bDontLight = false;
				mesh->bIsVisible = false;
				mesh->objColour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
				mesh->bIsWireFrame = true;
				mesh->specular = glm::vec4(1.0f, 1.0f, 1.0f, 1000.0f);

				sTextureInfo textureInfo;
				textureInfo.name = "rocks.bmp";
				textureInfo.strength = 1.0f;

				mesh->textures.push_back(textureInfo);
				this->addEntityToScene(new_entity);

				cMeshRenderSystem::getInstance()->registerEntity(new_entity);

				this->theMM.maze[a][b][0].center = def.position;
				this->theMM.maze[a][b][0].entity = new_entity;
				maze_info[this->theMM.maze[a][b][0].cube_id] = this->theMM.maze[a][b][0];


			}

		}
	}

}

void cScene::addEntityToScene(cEntity* entity) {
	this->entities.push_back(entity);
}

void cScene::removeEntityFromScene(cEntity* entity) {
	for (int i = 0; i < this->entities.size(); ++i)
	{
		if (this->entities[i] == entity)
		{
			this->entities.erase(this->entities.begin() + i);
			return;
		}
	}
}

cEntity* cScene::getSelectedEntity() {
	return this->entities[this->selectedEntity];
}

size_t cScene::getNumberOfEntites() {

	return this->entities.size();
}

cEntity* cScene::findObjectByFriendlyName(std::string theNameToFind) {

	for (unsigned int index = 0; index != this->entities.size(); index++) {

		cRenderMeshComponent* meshComp = static_cast<cRenderMeshComponent*>(this->entities[index]->components[3]);

		if (meshComp->friendlyName == theNameToFind)
		{
			return this->entities[index];
		}
	}

	return NULL;
}

cEntity* cScene::findObjectById(int id) {
	for (std::vector<cEntity*>::iterator it = this->entities.begin(); it != this->entities.end(); ++it) {

		if ((*it)->UNIQUE_ID == id) {
			return *it;
		}
	}

	return nullptr;
}

std::vector<cEntity*>& cScene::getEntities() {
	return this->entities;
}

void cScene::addLightToScene(sLight* light) {
	this->lights.push_back(light);
}

void cScene::removeLightFromScene(sLight* light) {

}

sLight* cScene::getSelectedLight() {
	return this->lights[this->selectedLight];
}

size_t cScene::getNumberOfLights() {
	return this->lights.size();
}

std::vector<sLight*> cScene::getLights() {
	return this->lights;
}

void cScene::addSoundToScene(cSoundManager::sSoundInfo* soundInfo) {
	this->sounds.push_back(soundInfo);
}

cSoundManager::sSoundInfo* cScene::getSoundInfo(unsigned index) {
	return this->sounds[index];
}

std::vector<cSoundManager::sSoundInfo*> cScene::getSounds() {
	return this->sounds;
}

void cScene::cycle_entities(bool forward)
{
	if (forward)
	{
		this->selectedEntity = (this->selectedEntity + 1) % this->entities.size();
	}
	else
	{
		this->selectedEntity -= 1;

		if (this->selectedEntity < 0)
		{
			this->selectedEntity = this->entities.size() - 1;
		}
	}

	////make sure it is a skin mesh
	if (entities[this->selectedEntity]->components[10] == nullptr)
	{
		this->cycle_entities(forward);
	}
}


double oldTime;
void cScene::handleCommand(iInputCommand* command) {

	double now = glfwGetTime();
	double delta = now - oldTime;
	oldTime = now;

	if (delta > 0.1f) {
		delta = 0.1f;
	}


	// camera controls
	cMoveCameraForwardCommand* moveCamZCommand = dynamic_cast<cMoveCameraForwardCommand*>(command);
	if (moveCamZCommand) {
		moveCamZCommand->execute(&this->flyCamera);
	}

	cMoveCameraLeftRightCommand* moveCamXCommand = dynamic_cast<cMoveCameraLeftRightCommand*>(command);
	if (moveCamXCommand) {
		moveCamXCommand->execute(&this->flyCamera);
	}

	cMoveCameraUpDownCommand* moveCamYCommand = dynamic_cast<cMoveCameraUpDownCommand*>(command);
	if (moveCamYCommand) {
		moveCamYCommand->execute(&this->flyCamera);
	}

	// light controls
	cMoveLightForwardCommand* moveLightForwardCommand = dynamic_cast<cMoveLightForwardCommand*>(command);
	if (moveLightForwardCommand) {
		moveLightForwardCommand->execute(this->getSelectedLight());
	}

	cMoveLightLeftRightCommand* moveLightLeftRightCommand = dynamic_cast<cMoveLightLeftRightCommand*>(command);
	if (moveLightLeftRightCommand) {
		moveLightLeftRightCommand->execute(this->getSelectedLight());
	}

	cMoveLightUpDownCommand* moveUpDownCommand = dynamic_cast<cMoveLightUpDownCommand*>(command);
	if (moveUpDownCommand) {
		moveUpDownCommand->execute(this->getSelectedLight());
	}

	//attenuation
	cLightChangeConstAttenuationCommand* changeLightConstAttenCommand = dynamic_cast<cLightChangeConstAttenuationCommand*>(command);
	if (changeLightConstAttenCommand) {
		changeLightConstAttenCommand->execute(this->getSelectedLight());
	}

	cLightChangeLinAttenuationCommand* changeLightLinAttenCommand = dynamic_cast<cLightChangeLinAttenuationCommand*>(command);
	if (changeLightLinAttenCommand) {
		changeLightLinAttenCommand->execute(this->getSelectedLight());
	}

	cLightChangeQuadAttenuationCommand* changeLightQuadAttenCommand = dynamic_cast<cLightChangeQuadAttenuationCommand*>(command);
	if (changeLightQuadAttenCommand) {
		changeLightQuadAttenCommand->execute(this->getSelectedLight());
	}

	// entity controls
	cTransformComponent* selectedEntityTransformComp = (cTransformComponent*)this->getSelectedEntity()->components[1];
	cRenderMeshComponent* selectedEntityMeshComp = (cRenderMeshComponent*)this->getSelectedEntity()->components[3];

	cMoveEntiyForwardCommand* moveEntityZCommand = dynamic_cast<cMoveEntiyForwardCommand*>(command);
	if (moveEntityZCommand) {
		moveEntityZCommand->execute(selectedEntityTransformComp);
	}

	cRotateEntiyXCommand* moveEntityRotXCommand = dynamic_cast<cRotateEntiyXCommand*>(command);
	if (moveEntityRotXCommand) {
		moveEntityRotXCommand->execute(selectedEntityTransformComp);
	}

	cRotateEntiyYCommand* moveEntityRotYCommand = dynamic_cast<cRotateEntiyYCommand*>(command);
	if (moveEntityRotYCommand) {
		moveEntityRotYCommand->execute(selectedEntityTransformComp);
	}

	cRotateEntiyZCommand* moveEntityRotZCommand = dynamic_cast<cRotateEntiyZCommand*>(command);
	if (moveEntityRotZCommand) {
		moveEntityRotZCommand->execute(selectedEntityTransformComp);
	}

	// specular power
	cChangeEntitySpecularPower* moveEntityChangeSpecPowerCommand = dynamic_cast<cChangeEntitySpecularPower*>(command);
	if (moveEntityChangeSpecPowerCommand) {
		moveEntityChangeSpecPowerCommand->execute(selectedEntityMeshComp);
	}

	//end the command's lifecycle
	delete command;
}

bool cScene::serialize() {

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

	writer.StartObject();
	writer.String("sceneName");
	writer.String(this->sceneName.c_str());
	writer.String("selectedEntity");
	writer.Int(this->selectedEntity);
	writer.String("selectedLight");
	writer.Int(this->selectedLight);

	writer.String(("camera"));
	writer.StartObject();

	writer.String("position");
	writer.StartObject();
	writer.String("x");
	writer.Double(this->flyCamera.eye.x);
	writer.String("y");
	writer.Double(this->flyCamera.eye.y);
	writer.String("z");
	writer.Double(this->flyCamera.eye.z);
	writer.EndObject();

	writer.String("orientation");
	writer.StartObject();
	writer.String("x");
	writer.Double(this->flyCamera.getQOrientation().x);
	writer.String("y");
	writer.Double(this->flyCamera.getQOrientation().y);
	writer.String("z");
	writer.Double(this->flyCamera.getQOrientation().z);
	writer.String("w");
	writer.Double(this->flyCamera.getQOrientation().w);
	writer.EndObject();

	writer.String("up");
	writer.StartObject();
	writer.String("x");
	writer.Double(this->flyCamera.getUpVector().x);
	writer.String("y");
	writer.Double(this->flyCamera.getUpVector().y);
	writer.String("z");
	writer.Double(this->flyCamera.getUpVector().z);
	writer.EndObject();
	writer.EndObject();


	writer.String(("lights"));
	writer.StartArray();
	for (std::vector<sLight*>::iterator light = this->lights.begin(); light != this->lights.end(); ++light) {
		(*light)->serializeComponent(writer);
	}
	writer.EndArray();

	writer.String(("entities"));
	writer.StartArray();
	for (std::vector<cEntity*>::iterator entity = this->entities.begin(); entity != this->entities.end(); ++entity) {
		(*entity)->serializeComponent(writer);
	}
	writer.EndArray();

	writer.String(("sounds"));
	writer.StartArray();
	for (std::vector<cSoundManager::sSoundInfo*>::iterator sound = this->sounds.begin(); sound != this->sounds.end(); ++sound) {
		writer.StartObject();
		(*sound)->serializeSound(writer);
		writer.EndObject();
	}
	writer.EndArray();

	writer.EndObject();

	// Write the JSON object to a file
	std::ofstream file("./assets/maps/" + this->sceneName);

	if (!file.is_open()) {
		return false;
	}
	file << buffer.GetString();

	return true;
}

bool cScene::deserialize(std::string filename) {

	this->sceneName = filename;
	std::ifstream ifs("./assets/maps/" + filename);

	if (!ifs.is_open()) {
		return false;
	}

	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document document;
	document.ParseStream(isw);

	//scene name
	assert(document.HasMember("sceneName"));
	assert(document["sceneName"].IsString());
	this->sceneName = document["sceneName"].GetString();

	//selected entity
	assert(document.HasMember("selectedEntity"));
	assert(document["selectedEntity"].IsInt());
	this->selectedEntity = document["selectedEntity"].GetInt();

	//selected light
	assert(document.HasMember("selectedLight"));
	assert(document["selectedLight"].IsInt());
	this->selectedLight = document["selectedLight"].GetInt();

	//camera
	assert(document.HasMember("camera"));
	assert(document["camera"].IsObject());

	// position
	assert(document["camera"].HasMember("position"));
	assert(document["camera"]["position"].IsObject());

	assert(document["camera"]["position"].HasMember("x"));
	assert(document["camera"]["position"]["x"].IsDouble());
	this->flyCamera.eye.x = (float)document["camera"]["position"]["x"].GetDouble();

	assert(document["camera"]["position"].HasMember("y"));
	assert(document["camera"]["position"]["y"].IsDouble());
	this->flyCamera.eye.y = (float)document["camera"]["position"]["y"].GetDouble();

	assert(document["camera"]["position"].HasMember("z"));
	assert(document["camera"]["position"]["z"].IsDouble());
	this->flyCamera.eye.z = (float)document["camera"]["position"]["z"].GetDouble();

	// look at
	glm::quat orientation;
	assert(document["camera"].HasMember("orientation"));
	assert(document["camera"]["orientation"].IsObject());

	assert(document["camera"]["orientation"].HasMember("x"));
	assert(document["camera"]["orientation"]["x"].IsDouble());
	orientation.x = (float)document["camera"]["orientation"]["x"].GetDouble();

	assert(document["camera"]["orientation"].HasMember("y"));
	assert(document["camera"]["orientation"]["y"].IsDouble());
	orientation.y = (float)document["camera"]["orientation"]["y"].GetDouble();

	assert(document["camera"]["orientation"].HasMember("z"));
	assert(document["camera"]["orientation"]["z"].IsDouble());
	orientation.z = (float)document["camera"]["orientation"]["z"].GetDouble();

	assert(document["camera"]["orientation"].HasMember("w"));
	assert(document["camera"]["orientation"]["w"].IsDouble());
	orientation.w = (float)document["camera"]["orientation"]["w"].GetDouble();

	this->flyCamera.adjMeshOrientationQ(orientation);


	// camera up
	assert(document["camera"].HasMember("up"));
	assert(document["camera"]["up"].IsObject());

	assert(document["camera"]["up"].HasMember("x"));
	assert(document["camera"]["up"]["x"].IsDouble());
	this->cameraUp.x = (float)document["camera"]["up"]["x"].GetDouble();

	assert(document["camera"]["up"].HasMember("y"));
	assert(document["camera"]["up"]["y"].IsDouble());
	this->cameraUp.y = (float)document["camera"]["up"]["y"].GetDouble();

	assert(document["camera"]["up"].HasMember("z"));
	assert(document["camera"]["up"]["z"].IsDouble());
	this->cameraUp.z = (float)document["camera"]["up"]["z"].GetDouble();

	//lights
	assert(document.HasMember("lights"));
	assert(document["lights"].IsArray());
	rapidjson::GenericArray < false, rapidjson::Value::ValueType> lights = (document["lights"].GetArray());

	unsigned int cameraCount = 0;
	cLightManager* pLightManager = cLightManager::getInstance();
	for (rapidjson::Value::ConstValueIterator itr = lights.Begin(); itr != lights.End(); ++itr) { // Ok
		sLight* light = pLightManager->getLightByIndex(cameraCount);

		//position
		assert(itr->HasMember("position"));
		assert((*itr)["position"].IsObject());

		assert((*itr)["position"].HasMember("x"));
		assert((*itr)["position"]["x"].IsDouble());
		light->position.x = (float)(*itr)["position"]["x"].GetDouble();

		assert((*itr)["position"].HasMember("y"));
		assert((*itr)["position"]["y"].IsDouble());
		light->position.y = (float)(*itr)["position"]["y"].GetDouble();

		assert((*itr)["position"].HasMember("z"));
		assert((*itr)["position"]["z"].IsDouble());
		light->position.z = (float)(*itr)["position"]["z"].GetDouble();

		//specular
		assert(itr->HasMember("specular"));
		assert((*itr)["specular"].IsObject());

		assert((*itr)["specular"].HasMember("r"));
		assert((*itr)["specular"]["r"].IsDouble());
		light->specular.x = (float)(*itr)["specular"]["r"].GetDouble();

		assert((*itr)["specular"].HasMember("g"));
		assert((*itr)["specular"]["g"].IsDouble());
		light->specular.y = (float)(*itr)["specular"]["g"].GetDouble();

		assert((*itr)["specular"].HasMember("b"));
		assert((*itr)["specular"]["b"].IsDouble());
		light->specular.z = (float)(*itr)["specular"]["b"].GetDouble();

		assert((*itr)["specular"].HasMember("a"));
		assert((*itr)["specular"]["a"].IsDouble());
		light->specular.w = (float)(*itr)["specular"]["a"].GetDouble();

		//diffuse
		assert(itr->HasMember("diffuse"));
		assert((*itr)["diffuse"].IsObject());

		assert((*itr)["diffuse"].HasMember("r"));
		assert((*itr)["diffuse"]["r"].IsDouble());
		light->diffuse.x = (float)(*itr)["diffuse"]["r"].GetDouble();

		assert((*itr)["diffuse"].HasMember("g"));
		assert((*itr)["diffuse"]["g"].IsDouble());
		light->diffuse.y = (float)(*itr)["diffuse"]["g"].GetDouble();

		assert((*itr)["diffuse"].HasMember("b"));
		assert((*itr)["diffuse"]["b"].IsDouble());
		light->diffuse.z = (float)(*itr)["diffuse"]["b"].GetDouble();

		assert((*itr)["diffuse"].HasMember("a"));
		assert((*itr)["diffuse"]["a"].IsDouble());
		light->diffuse.w = (float)(*itr)["diffuse"]["a"].GetDouble();

		//attenuation
		assert(itr->HasMember("attenuation"));
		assert((*itr)["attenuation"].IsObject());

		assert((*itr)["attenuation"].HasMember("constant"));
		assert((*itr)["attenuation"]["constant"].IsDouble());
		light->atten.x = (float)(*itr)["attenuation"]["constant"].GetDouble();

		assert((*itr)["attenuation"].HasMember("linear"));
		assert((*itr)["attenuation"]["linear"].IsDouble());
		light->atten.y = (float)(*itr)["attenuation"]["linear"].GetDouble();

		assert((*itr)["attenuation"].HasMember("quadratic"));
		assert((*itr)["attenuation"]["quadratic"].IsDouble());
		light->atten.z = (float)(*itr)["attenuation"]["quadratic"].GetDouble();

		assert((*itr)["attenuation"].HasMember("cutoffDist"));
		assert((*itr)["attenuation"]["cutoffDist"].IsDouble());
		light->atten.w = (float)(*itr)["attenuation"]["cutoffDist"].GetDouble();

		//direction
		assert(itr->HasMember("direction"));
		assert((*itr)["direction"].IsObject());

		assert((*itr)["direction"].HasMember("x"));
		assert((*itr)["direction"]["x"].IsDouble());
		light->direction.x = (float)(*itr)["direction"]["x"].GetDouble();

		assert((*itr)["direction"].HasMember("y"));
		assert((*itr)["direction"]["y"].IsDouble());
		light->direction.y = (float)(*itr)["direction"]["y"].GetDouble();

		assert((*itr)["direction"].HasMember("z"));
		assert((*itr)["direction"]["z"].IsDouble());
		light->direction.z = (float)(*itr)["direction"]["z"].GetDouble();

		assert((*itr)["direction"].HasMember("w"));
		assert((*itr)["direction"]["w"].IsDouble());
		light->direction.w = (float)(*itr)["direction"]["w"].GetDouble();

		//param1
		assert(itr->HasMember("param1"));
		assert((*itr)["param1"].IsObject());

		assert((*itr)["param1"].HasMember("lightType"));
		assert((*itr)["param1"]["lightType"].IsDouble());
		light->param1.x = (float)(*itr)["param1"]["lightType"].GetDouble();

		assert((*itr)["param1"].HasMember("innerAngle"));
		assert((*itr)["param1"]["innerAngle"].IsDouble());
		light->param1.y = (float)(*itr)["param1"]["innerAngle"].GetDouble();

		assert((*itr)["param1"].HasMember("outerAngle"));
		assert((*itr)["param1"]["outerAngle"].IsDouble());
		light->param1.z = (float)(*itr)["param1"]["outerAngle"].GetDouble();

		assert((*itr)["param1"].HasMember("w"));
		assert((*itr)["param1"]["w"].IsDouble());
		light->param1.w = (float)(*itr)["param1"]["w"].GetDouble();

		//param2
		assert(itr->HasMember("param2"));
		assert((*itr)["param2"].IsObject());

		assert((*itr)["param2"].HasMember("isOn"));
		assert((*itr)["param2"]["isOn"].IsDouble());
		light->param2.x = (float)(*itr)["param2"]["isOn"].GetDouble();

		assert((*itr)["param2"].HasMember("y"));
		assert((*itr)["param2"]["y"].IsDouble());
		light->param2.y = (float)(*itr)["param2"]["y"].GetDouble();

		assert((*itr)["param2"].HasMember("z"));
		assert((*itr)["param2"]["z"].IsDouble());
		light->param2.z = (float)(*itr)["param2"]["z"].GetDouble();

		assert((*itr)["param2"].HasMember("w"));
		assert((*itr)["param2"]["w"].IsDouble());
		light->param2.z = (float)(*itr)["param2"]["w"].GetDouble();

		this->addLightToScene(light);

		cameraCount++;
	}

	// sounds non-3d
	assert(document.HasMember("sounds"));
	assert(document["sounds"].IsArray());

	rapidjson::GenericArray < false, rapidjson::Value::ValueType> sounds = (document["sounds"].GetArray());

	for (rapidjson::Value::ConstValueIterator compItr = sounds.Begin(); compItr != sounds.End(); ++compItr) {

		cSoundManager::sSoundInfo* soundInfo = new cSoundManager::sSoundInfo();

		//acceleration
		assert(compItr->HasMember("fileName"));
		assert((*compItr)["fileName"].IsString());
		soundInfo->fileName = (*compItr)["fileName"].GetString();

		cSoundManager::getInstance()->load3dSound(soundInfo, FMOD_LOOP_NORMAL | FMOD_3D_LINEARROLLOFF);

		assert(compItr->HasMember("soundType"));
		assert((*compItr)["soundType"].IsInt());
		soundInfo->type = (*compItr)["soundType"].GetInt();

		assert(compItr->HasMember("channelGroup"));
		assert((*compItr)["channelGroup"].IsInt());
		soundInfo->channelGroupIndex = (*compItr)["channelGroup"].GetInt();

		assert(compItr->HasMember("isPaused"));
		assert((*compItr)["isPaused"].IsBool());
		soundInfo->isPaused = (*compItr)["isPaused"].GetBool();

		assert(compItr->HasMember("isCompressed"));
		assert((*compItr)["isCompressed"].IsBool());
		soundInfo->isCompressed = (*compItr)["isCompressed"].GetBool();

		assert(compItr->HasMember("frequency"));
		assert((*compItr)["frequency"].IsDouble());
		soundInfo->frequency = (float)(*compItr)["frequency"].GetDouble();

		assert(compItr->HasMember("volume"));
		assert((*compItr)["volume"].IsDouble());
		soundInfo->volume = (float)(*compItr)["volume"].GetDouble();

		assert(compItr->HasMember("balance"));
		assert((*compItr)["balance"].IsDouble());
		soundInfo->balance = (float)(*compItr)["balance"].GetDouble();

		assert(compItr->HasMember("pitch"));
		assert((*compItr)["pitch"].IsDouble());
		soundInfo->pitch = (float)(*compItr)["pitch"].GetDouble();

		assert(compItr->HasMember("soundLength"));
		assert((*compItr)["soundLength"].IsUint());
		soundInfo->soundLength = (*compItr)["soundLength"].GetUint();

		assert(compItr->HasMember("channelPosition"));
		assert((*compItr)["channelPosition"].IsUint());
		soundInfo->channelPosition = (*compItr)["channelPosition"].GetUint();

		assert(compItr->HasMember("minDistance"));
		assert((*compItr)["minDistance"].IsDouble());
		soundInfo->f3dMinDistance = (float)(*compItr)["minDistance"].GetDouble();

		assert(compItr->HasMember("maxDistance"));
		assert((*compItr)["maxDistance"].IsDouble());
		soundInfo->f3dMaxDistance = (float)(*compItr)["maxDistance"].GetDouble();

		// "play the sound" to get a channels and others stuff
		cSoundManager::getInstance()->playSound(soundInfo);

		cSoundManager::getInstance()->setPaused(soundInfo, soundInfo->isPaused);
		cSoundManager::getInstance()->setSoundFrequency(*soundInfo, soundInfo->frequency);
		cSoundManager::getInstance()->setSoundVolume(*soundInfo, soundInfo->volume);
		cSoundManager::getInstance()->setSoundPitch(*soundInfo, soundInfo->pitch);
		cSoundManager::getInstance()->setSoundCurrentPosition(*soundInfo, soundInfo->channelPosition);

		this->addSoundToScene(soundInfo);

	}

	//entities
	assert(document.HasMember("entities"));
	assert(document["entities"].IsArray());
	rapidjson::GenericArray < false, rapidjson::Value::ValueType> entities = (document["entities"].GetArray()); //this is super templated library (the type is crazy)
	for (rapidjson::Value::ConstValueIterator itr = entities.Begin(); itr != entities.End(); ++itr) { // Ok

		assert((*itr).HasMember("entityType"));
		assert((*itr)["entityType"].IsInt());

		// create an instance of the entity
		cEntity* entity = cEntityBuilder::getInstance()->createEntity((*itr)["entityType"].GetInt());

		assert(itr->HasMember("id"));
		assert((*itr)["id"].IsInt());
		entity->UNIQUE_ID = (*itr)["id"].GetInt();

		// update the components to match the file
		assert((*itr).HasMember("components"));
		assert((*itr)["components"].IsArray());
		auto components = ((*itr)["components"].GetArray()); //this is super templated library (the type is crazy)
		for (rapidjson::Value::ConstValueIterator compItr = components.Begin(); compItr != components.End(); ++compItr) {

			assert((*compItr).HasMember("componentType"));
			assert((*compItr)["componentType"].IsInt());

			switch ((*compItr)["componentType"].GetInt()) {
			case 1:
			{
				cTransformComponent* transformComp = (cTransformComponent*)(entity->components[1]);

				//position
				assert(compItr->HasMember("position"));
				assert((*compItr)["position"].IsObject());

				glm::vec3 pos;
				assert((*compItr)["position"].HasMember("x"));
				assert((*compItr)["position"]["x"].IsDouble());
				pos.x = (float)(*compItr)["position"]["x"].GetDouble();

				assert((*compItr)["position"].HasMember("y"));
				assert((*compItr)["position"]["y"].IsDouble());
				pos.y = (float)(*compItr)["position"]["y"].GetDouble();

				assert((*compItr)["position"].HasMember("z"));
				assert((*compItr)["position"]["z"].IsDouble());
				pos.z = (float)(*compItr)["position"]["z"].GetDouble();

				transformComp->set_position(pos);

				//rotation
				assert(compItr->HasMember("orientation"));
				assert((*compItr)["orientation"].IsObject());

				glm::vec3 eulerOrientation;

				assert((*compItr)["orientation"].HasMember("x"));
				assert((*compItr)["orientation"]["x"].IsDouble());
				eulerOrientation.x = (float)(*compItr)["orientation"]["x"].GetDouble();

				assert((*compItr)["orientation"].HasMember("y"));
				assert((*compItr)["orientation"]["y"].IsDouble());
				eulerOrientation.y = (float)(*compItr)["orientation"]["y"].GetDouble();

				assert((*compItr)["orientation"].HasMember("z"));
				assert((*compItr)["orientation"]["z"].IsDouble());
				eulerOrientation.z = (float)(*compItr)["orientation"]["z"].GetDouble();

				transformComp->setOrientationEulerAngles(eulerOrientation, true);

				//scale
				assert(compItr->HasMember("scale"));
				assert((*compItr)["scale"].IsObject());

				assert((*compItr)["scale"].HasMember("x"));
				assert((*compItr)["scale"]["x"].IsDouble());
				transformComp->scale.x = (float)(*compItr)["scale"]["x"].GetDouble();

				assert((*compItr)["scale"].HasMember("y"));
				assert((*compItr)["scale"]["y"].IsDouble());
				transformComp->scale.y = (float)(*compItr)["scale"]["y"].GetDouble();

				assert((*compItr)["scale"].HasMember("z"));
				assert((*compItr)["scale"]["z"].IsDouble());
				transformComp->scale.z = (float)(*compItr)["scale"]["z"].GetDouble();

				this->addEntityToScene(entity);
				break;
			}

			case 2:
			{
				cEulerMovementComponent* eulerMoveComp = (cEulerMovementComponent*)(entity->components[2]);

				//acceleration
				assert(compItr->HasMember("acceleration"));
				assert((*compItr)["acceleration"].IsObject());

				assert((*compItr)["acceleration"].HasMember("x"));
				assert((*compItr)["acceleration"]["x"].IsDouble());
				eulerMoveComp->acceleration.x = (float)(*compItr)["acceleration"]["x"].GetDouble();

				assert((*compItr)["acceleration"].HasMember("y"));
				assert((*compItr)["acceleration"]["y"].IsDouble());
				eulerMoveComp->acceleration.y = (float)(*compItr)["acceleration"]["y"].GetDouble();

				assert((*compItr)["acceleration"].HasMember("z"));
				assert((*compItr)["acceleration"]["z"].IsDouble());
				eulerMoveComp->acceleration.z = (float)(*compItr)["acceleration"]["z"].GetDouble();

				//velocity
				assert(compItr->HasMember("velocity"));
				assert((*compItr)["velocity"].IsObject());

				assert((*compItr)["velocity"].HasMember("x"));
				assert((*compItr)["velocity"]["x"].IsDouble());
				eulerMoveComp->velocity.x = (float)(*compItr)["velocity"]["x"].GetDouble();

				assert((*compItr)["velocity"].HasMember("y"));
				assert((*compItr)["velocity"]["y"].IsDouble());
				eulerMoveComp->velocity.y = (float)(*compItr)["velocity"]["y"].GetDouble();

				assert((*compItr)["velocity"].HasMember("z"));
				assert((*compItr)["velocity"]["z"].IsDouble());
				eulerMoveComp->velocity.z = (float)(*compItr)["velocity"]["z"].GetDouble();

				break;
			}

			case 3:
			{
				cRenderMeshComponent* renderComp = (cRenderMeshComponent*)(entity->components[3]);

				//acceleration
				assert(compItr->HasMember("meshName"));
				assert((*compItr)["meshName"].IsString());
				renderComp->meshName = (*compItr)["meshName"].GetString();


				assert(compItr->HasMember("isWireframe"));
				assert((*compItr)["isWireframe"].IsBool());
				renderComp->bIsWireFrame = (*compItr)["isWireframe"].GetBool();

				assert(compItr->HasMember("isVisible"));
				assert((*compItr)["isVisible"].IsBool());
				renderComp->bIsVisible = (*compItr)["isVisible"].GetBool();

				assert(compItr->HasMember("useVertexColor"));
				assert((*compItr)["useVertexColor"].IsBool());
				renderComp->bUseVertexColour = (*compItr)["useVertexColor"].GetBool();

				assert(compItr->HasMember("dontLight"));
				assert((*compItr)["dontLight"].IsBool());
				renderComp->bDontLight = (*compItr)["dontLight"].GetBool();

				assert(compItr->HasMember("color"));
				assert((*compItr)["color"].IsObject());

				assert((*compItr)["color"].HasMember("r"));
				assert((*compItr)["color"]["r"].IsDouble());
				renderComp->objColour.r = (float)(*compItr)["color"]["r"].GetDouble();

				assert((*compItr)["color"].HasMember("g"));
				assert((*compItr)["color"]["g"].IsDouble());
				renderComp->objColour.g = (float)(*compItr)["color"]["g"].GetDouble();

				assert((*compItr)["color"].HasMember("b"));
				assert((*compItr)["color"]["b"].IsDouble());
				renderComp->objColour.b = (float)(*compItr)["color"]["b"].GetDouble();

				assert((*compItr)["color"].HasMember("a"));
				assert((*compItr)["color"]["a"].IsDouble());
				renderComp->objColour.a = (float)(*compItr)["color"]["a"].GetDouble();

				//textures
				assert(compItr->HasMember("textures"));
				assert((*compItr)["textures"].IsArray());
				rapidjson::GenericArray < true, rapidjson::Value::ValueType> textures = ((*compItr)["textures"].GetArray());

				for (rapidjson::Value::ConstValueIterator texture = textures.Begin(); texture != textures.End(); ++texture) {
					sTextureInfo textureInfo;

					assert(texture->HasMember("name"));
					assert((*texture)["name"].IsString());
					textureInfo.name = (*texture)["name"].GetString();

					assert(texture->HasMember("strength"));
					assert((*texture)["strength"].IsDouble());
					textureInfo.strength = (float)(*texture)["strength"].GetDouble();

					renderComp->textures.push_back(textureInfo);
				}

				if (compItr->HasMember("bUseSplatMap"))
				{
					assert((*compItr)["bUseSplatMap"].IsBool());
					renderComp->bUseSplatMap = (*compItr)["bUseSplatMap"].GetBool();
				}


				assert(compItr->HasMember("specular"));
				assert((*compItr)["specular"].IsObject());

				assert((*compItr)["specular"].HasMember("r"));
				assert((*compItr)["specular"]["r"].IsDouble());
				renderComp->specular.r = (float)(*compItr)["specular"]["r"].GetDouble();

				assert((*compItr)["specular"].HasMember("g"));
				assert((*compItr)["specular"]["g"].IsDouble());
				renderComp->specular.g = (float)(*compItr)["specular"]["g"].GetDouble();

				assert((*compItr)["specular"].HasMember("b"));
				assert((*compItr)["specular"]["b"].IsDouble());
				renderComp->specular.b = (float)(*compItr)["specular"]["b"].GetDouble();

				assert((*compItr)["specular"].HasMember("shinyness"));
				assert((*compItr)["specular"]["shinyness"].IsDouble());
				renderComp->specular.a = (float)(*compItr)["specular"]["shinyness"].GetDouble();

				break;
			}

			case 6:
			{
				c3dSoundComponent* soundComp = (c3dSoundComponent*)(entity->components[6]);

				//acceleration
				assert(compItr->HasMember("fileName"));
				assert((*compItr)["fileName"].IsString());
				soundComp->soundInfo->fileName = (*compItr)["fileName"].GetString();

				cSoundManager::getInstance()->load3dSound(soundComp->soundInfo, FMOD_LOOP_NORMAL | FMOD_3D_LINEARROLLOFF);

				assert(compItr->HasMember("soundType"));
				assert((*compItr)["soundType"].IsInt());
				soundComp->soundInfo->type = (*compItr)["soundType"].GetInt();

				assert(compItr->HasMember("channelGroup"));
				assert((*compItr)["channelGroup"].IsInt());
				soundComp->soundInfo->channelGroupIndex = (*compItr)["channelGroup"].GetInt();

				assert(compItr->HasMember("isPaused"));
				assert((*compItr)["isPaused"].IsBool());
				soundComp->soundInfo->isPaused = (*compItr)["isPaused"].GetBool();

				assert(compItr->HasMember("isCompressed"));
				assert((*compItr)["isCompressed"].IsBool());
				soundComp->soundInfo->isCompressed = (*compItr)["isCompressed"].GetBool();

				assert(compItr->HasMember("frequency"));
				assert((*compItr)["frequency"].IsDouble());
				soundComp->soundInfo->frequency = (float)(*compItr)["frequency"].GetDouble();

				assert(compItr->HasMember("volume"));
				assert((*compItr)["volume"].IsDouble());
				soundComp->soundInfo->volume = (float)(*compItr)["volume"].GetDouble();

				assert(compItr->HasMember("balance"));
				assert((*compItr)["balance"].IsDouble());
				soundComp->soundInfo->balance = (float)(*compItr)["balance"].GetDouble();

				assert(compItr->HasMember("pitch"));
				assert((*compItr)["pitch"].IsDouble());
				soundComp->soundInfo->pitch = (float)(*compItr)["pitch"].GetDouble();

				assert(compItr->HasMember("soundLength"));
				assert((*compItr)["soundLength"].IsUint());
				soundComp->soundInfo->soundLength = (*compItr)["soundLength"].GetUint();

				assert(compItr->HasMember("channelPosition"));
				assert((*compItr)["channelPosition"].IsUint());
				soundComp->soundInfo->channelPosition = (*compItr)["channelPosition"].GetUint();

				assert(compItr->HasMember("minDistance"));
				assert((*compItr)["minDistance"].IsDouble());
				soundComp->soundInfo->f3dMinDistance = (float)(*compItr)["minDistance"].GetDouble();

				assert(compItr->HasMember("maxDistance"));
				assert((*compItr)["maxDistance"].IsDouble());
				soundComp->soundInfo->f3dMaxDistance = (float)(*compItr)["maxDistance"].GetDouble();

				// "play the sound" to get a channels and others stuff
				cSoundManager::getInstance()->playSound(soundComp->soundInfo);

				//move the 3d sound
				cTransformComponent* transComp = (cTransformComponent*)entity->components[1];
				FMOD_VECTOR position;
				position.x = transComp->get_position().x;
				position.y = transComp->get_position().y;
				position.z = transComp->get_position().z;

				FMOD_VECTOR velocity = { 0.0f,0.0f,0.0f };

				cSoundManager::getInstance()->mresult = soundComp->soundInfo->channel->set3DAttributes(&position, &velocity);
				cSoundManager::getInstance()->checkError();

				cSoundManager::getInstance()->mresult = soundComp->soundInfo->channel->set3DMinMaxDistance(soundComp->soundInfo->f3dMinDistance, soundComp->soundInfo->f3dMaxDistance);
				cSoundManager::getInstance()->checkError();

				cSoundManager::getInstance()->setPaused(soundComp->soundInfo, soundComp->soundInfo->isPaused);
				cSoundManager::getInstance()->setSoundFrequency(*soundComp->soundInfo, soundComp->soundInfo->frequency);
				cSoundManager::getInstance()->setSoundVolume(*soundComp->soundInfo, soundComp->soundInfo->volume);
				cSoundManager::getInstance()->setSoundPitch(*soundComp->soundInfo, soundComp->soundInfo->pitch);
				cSoundManager::getInstance()->setSoundCurrentPosition(*soundComp->soundInfo, soundComp->soundInfo->channelPosition);

				cSoundManager::getInstance()->update();

				break;
			}
			case 7:
			{
				cParticleComponent* particleComp = static_cast<cParticleComponent*>(entity->components[7]);
				cParticleComponent::sParticleCreationParams particleParams;

				assert(compItr->HasMember("minLifeTime"));
				assert((*compItr)["minLifeTime"].IsDouble());
				particleParams.minLifeTime = (float)(*compItr)["minLifeTime"].GetDouble();

				assert(compItr->HasMember("maxLifeTime"));
				assert((*compItr)["maxLifeTime"].IsDouble());
				particleParams.maxLifeTime = (float)(*compItr)["maxLifeTime"].GetDouble();

				assert(compItr->HasMember("totalParticles"));
				assert((*compItr)["totalParticles"].IsUint());
				particleParams.totalNumberOfParticles = (*compItr)["totalParticles"].GetUint();

				//position
				assert(compItr->HasMember("position"));
				assert((*compItr)["position"].IsObject());

				assert((*compItr)["position"].HasMember("x"));
				assert((*compItr)["position"]["x"].IsDouble());
				particleParams.position.x = (float)(*compItr)["position"]["x"].GetDouble();

				assert((*compItr)["position"].HasMember("y"));
				assert((*compItr)["position"]["y"].IsDouble());
				particleParams.position.y = (float)(*compItr)["position"]["y"].GetDouble();

				assert((*compItr)["position"].HasMember("z"));
				assert((*compItr)["position"]["z"].IsDouble());
				particleParams.position.z = (float)(*compItr)["position"]["z"].GetDouble();

				// min intital velocity
				assert(compItr->HasMember("minInitialVelocity"));
				assert((*compItr)["minInitialVelocity"].IsObject());

				assert((*compItr)["minInitialVelocity"].HasMember("x"));
				assert((*compItr)["minInitialVelocity"]["x"].IsDouble());
				particleParams.minInitialVelocity.x = (float)(*compItr)["minInitialVelocity"]["x"].GetDouble();

				assert((*compItr)["minInitialVelocity"].HasMember("y"));
				assert((*compItr)["minInitialVelocity"]["y"].IsDouble());
				particleParams.minInitialVelocity.y = (float)(*compItr)["minInitialVelocity"]["y"].GetDouble();

				assert((*compItr)["minInitialVelocity"].HasMember("z"));
				assert((*compItr)["minInitialVelocity"]["z"].IsDouble());
				particleParams.minInitialVelocity.z = (float)(*compItr)["minInitialVelocity"]["z"].GetDouble();

				// max initial velocity
				assert(compItr->HasMember("maxInitialVelocity"));
				assert((*compItr)["maxInitialVelocity"].IsObject());

				assert((*compItr)["maxInitialVelocity"].HasMember("x"));
				assert((*compItr)["maxInitialVelocity"]["x"].IsDouble());
				particleParams.maxInitialVelocity.x = (float)(*compItr)["maxInitialVelocity"]["x"].GetDouble();

				assert((*compItr)["maxInitialVelocity"].HasMember("y"));
				assert((*compItr)["maxInitialVelocity"]["y"].IsDouble());
				particleParams.maxInitialVelocity.y = (float)(*compItr)["maxInitialVelocity"]["y"].GetDouble();

				assert((*compItr)["maxInitialVelocity"].HasMember("z"));
				assert((*compItr)["maxInitialVelocity"]["z"].IsDouble());
				particleParams.maxInitialVelocity.z = (float)(*compItr)["maxInitialVelocity"]["z"].GetDouble();

				// acceleration
				assert(compItr->HasMember("acceleration"));
				assert((*compItr)["acceleration"].IsObject());

				assert((*compItr)["acceleration"].HasMember("x"));
				assert((*compItr)["acceleration"]["x"].IsDouble());
				particleParams.acceleration.x = (float)(*compItr)["acceleration"]["x"].GetDouble();

				assert((*compItr)["acceleration"].HasMember("y"));
				assert((*compItr)["acceleration"]["y"].IsDouble());
				particleParams.acceleration.y = (float)(*compItr)["acceleration"]["y"].GetDouble();

				assert((*compItr)["acceleration"].HasMember("z"));
				assert((*compItr)["acceleration"]["z"].IsDouble());
				particleParams.acceleration.z = (float)(*compItr)["acceleration"]["z"].GetDouble();

				// min initial orientation
				assert(compItr->HasMember("minInitialOrientation"));
				assert((*compItr)["minInitialOrientation"].IsObject());

				assert((*compItr)["minInitialOrientation"].HasMember("x"));
				assert((*compItr)["minInitialOrientation"]["x"].IsDouble());
				particleParams.minInitialOrientationEuler.x = (float)(*compItr)["minInitialOrientation"]["x"].GetDouble();

				assert((*compItr)["minInitialOrientation"].HasMember("y"));
				assert((*compItr)["minInitialOrientation"]["y"].IsDouble());
				particleParams.minInitialOrientationEuler.y = (float)(*compItr)["minInitialOrientation"]["y"].GetDouble();

				assert((*compItr)["minInitialOrientation"].HasMember("z"));
				assert((*compItr)["minInitialOrientation"]["z"].IsDouble());
				particleParams.minInitialOrientationEuler.z = (float)(*compItr)["minInitialOrientation"]["z"].GetDouble();

				// max initial orientation
				assert(compItr->HasMember("maxInitialOrientation"));
				assert((*compItr)["maxInitialOrientation"].IsObject());

				assert((*compItr)["maxInitialOrientation"].HasMember("x"));
				assert((*compItr)["maxInitialOrientation"]["x"].IsDouble());
				particleParams.maxInitialOrientationEuler.x = (float)(*compItr)["maxInitialOrientation"]["x"].GetDouble();

				assert((*compItr)["maxInitialOrientation"].HasMember("y"));
				assert((*compItr)["maxInitialOrientation"]["y"].IsDouble());
				particleParams.maxInitialOrientationEuler.y = (float)(*compItr)["maxInitialOrientation"]["y"].GetDouble();

				assert((*compItr)["maxInitialOrientation"].HasMember("z"));
				assert((*compItr)["maxInitialOrientation"]["z"].IsDouble());
				particleParams.maxInitialOrientationEuler.z = (float)(*compItr)["maxInitialOrientation"]["z"].GetDouble();

				// min orientation change
				assert(compItr->HasMember("minOrientationChange"));
				assert((*compItr)["minOrientationChange"].IsObject());

				assert((*compItr)["minOrientationChange"].HasMember("x"));
				assert((*compItr)["minOrientationChange"]["x"].IsDouble());
				particleParams.minOrientationChangeEuler.x = (float)(*compItr)["minOrientationChange"]["x"].GetDouble();

				assert((*compItr)["minOrientationChange"].HasMember("y"));
				assert((*compItr)["minOrientationChange"]["y"].IsDouble());
				particleParams.minOrientationChangeEuler.y = (float)(*compItr)["minOrientationChange"]["y"].GetDouble();

				assert((*compItr)["minOrientationChange"].HasMember("z"));
				assert((*compItr)["minOrientationChange"]["z"].IsDouble());
				particleParams.minOrientationChangeEuler.z = (float)(*compItr)["minOrientationChange"]["z"].GetDouble();

				// max orientation change
				assert(compItr->HasMember("maxOrientationChange"));
				assert((*compItr)["maxOrientationChange"].IsObject());

				assert((*compItr)["maxOrientationChange"].HasMember("x"));
				assert((*compItr)["maxOrientationChange"]["x"].IsDouble());
				particleParams.maxOrientationChangeEuler.x = (float)(*compItr)["maxOrientationChange"]["x"].GetDouble();

				assert((*compItr)["maxOrientationChange"].HasMember("y"));
				assert((*compItr)["maxOrientationChange"]["y"].IsDouble());
				particleParams.maxOrientationChangeEuler.y = (float)(*compItr)["maxOrientationChange"]["y"].GetDouble();

				assert((*compItr)["maxOrientationChange"].HasMember("z"));
				assert((*compItr)["maxOrientationChange"]["z"].IsDouble());
				particleParams.maxOrientationChangeEuler.z = (float)(*compItr)["maxOrientationChange"]["z"].GetDouble();

				assert(compItr->HasMember("minInitialScale"));
				assert((*compItr)["minInitialScale"].IsDouble());
				particleParams.minInitialScale = (float)(*compItr)["minInitialScale"].GetDouble();

				assert(compItr->HasMember("maxInitialScale"));
				assert((*compItr)["maxInitialScale"].IsDouble());
				particleParams.maxInitialScale = (float)(*compItr)["maxInitialScale"].GetDouble();

				assert(compItr->HasMember("minScaleChange"));
				assert((*compItr)["minScaleChange"].IsDouble());
				particleParams.minScaleChange = (float)(*compItr)["minScaleChange"].GetDouble();

				assert(compItr->HasMember("maxScaleChange"));
				assert((*compItr)["maxScaleChange"].IsDouble());
				particleParams.maxScaleChange = (float)(*compItr)["maxScaleChange"].GetDouble();

				assert(compItr->HasMember("deathFadeTime"));
				assert((*compItr)["deathFadeTime"].IsDouble());
				particleParams.deathTransparencyFadeTimeSeconds = (float)(*compItr)["deathFadeTime"].GetDouble();

				assert(compItr->HasMember("minNewParticles"));
				assert((*compItr)["minNewParticles"].IsUint());
				particleParams.minNumberNewParticles = (*compItr)["minNewParticles"].GetUint();

				assert(compItr->HasMember("maxNewParticles"));
				assert((*compItr)["maxNewParticles"].IsUint());
				particleParams.maxNumberNewParticles = (*compItr)["maxNewParticles"].GetUint();

				assert(compItr->HasMember("minTimeBetweenGen"));
				assert((*compItr)["minTimeBetweenGen"].IsDouble());
				particleParams.minTimeBetweenParticleGenSeconds = (float)(*compItr)["minTimeBetweenGen"].GetDouble();

				assert(compItr->HasMember("maxTimeBetweenGen"));
				assert((*compItr)["maxTimeBetweenGen"].IsDouble());
				particleParams.maxTimeBetweenParticleGenSeconds = (float)(*compItr)["maxTimeBetweenGen"].GetDouble();

				particleComp->init(particleParams);
			}
			break;

			//NEW PHYSICS STUFF
			case 8:
			{
				cPhysicsComponent* physics = static_cast<cPhysicsComponent*>(entity->components[8]);
				physics::defs::sRigidBodyDef def;

				//position
				assert(compItr->HasMember("position"));
				assert((*compItr)["position"].IsObject());

				assert((*compItr)["position"].HasMember("x"));
				assert((*compItr)["position"]["x"].IsDouble());
				def.position.x = (float)(*compItr)["position"]["x"].GetDouble();

				assert((*compItr)["position"].HasMember("y"));
				assert((*compItr)["position"]["y"].IsDouble());
				def.position.y = (float)(*compItr)["position"]["y"].GetDouble();

				assert((*compItr)["position"].HasMember("z"));
				assert((*compItr)["position"]["z"].IsDouble());
				def.position.z = (float)(*compItr)["position"]["z"].GetDouble();

				//scale
				assert(compItr->HasMember("scale"));
				assert((*compItr)["scale"].IsObject());

				assert((*compItr)["scale"].HasMember("x"));
				assert((*compItr)["scale"]["x"].IsDouble());
				def.scale.x = (float)(*compItr)["scale"]["x"].GetDouble();

				assert((*compItr)["scale"].HasMember("y"));
				assert((*compItr)["scale"]["y"].IsDouble());
				def.scale.y = (float)(*compItr)["scale"]["y"].GetDouble();

				assert((*compItr)["scale"].HasMember("z"));
				assert((*compItr)["scale"]["z"].IsDouble());
				def.scale.z = (float)(*compItr)["scale"]["z"].GetDouble();

				//orientation
				assert(compItr->HasMember("orientation"));
				assert((*compItr)["orientation"].IsObject());

				assert((*compItr)["orientation"].HasMember("x"));
				assert((*compItr)["orientation"]["x"].IsDouble());
				def.orientation.x = (float)(*compItr)["orientation"]["x"].GetDouble();

				assert((*compItr)["orientation"].HasMember("y"));
				assert((*compItr)["orientation"]["y"].IsDouble());
				def.orientation.y = (float)(*compItr)["orientation"]["y"].GetDouble();

				assert((*compItr)["orientation"].HasMember("z"));
				assert((*compItr)["orientation"]["z"].IsDouble());
				def.orientation.z = (float)(*compItr)["orientation"]["z"].GetDouble();

				//mass
				assert(compItr->HasMember("mass"));
				assert((*compItr)["mass"].IsDouble());
				def.mass = (*compItr)["mass"].GetDouble();

				//acceleration
				assert(compItr->HasMember("acceleration"));
				assert((*compItr)["acceleration"].IsObject());

				assert((*compItr)["acceleration"].HasMember("x"));
				assert((*compItr)["acceleration"]["x"].IsDouble());
				def.acceleration.x = (float)(*compItr)["acceleration"]["x"].GetDouble();

				assert((*compItr)["acceleration"].HasMember("y"));
				assert((*compItr)["acceleration"]["y"].IsDouble());
				def.acceleration.y = (float)(*compItr)["acceleration"]["y"].GetDouble();

				assert((*compItr)["acceleration"].HasMember("z"));
				assert((*compItr)["acceleration"]["z"].IsDouble());
				def.acceleration.z = (float)(*compItr)["acceleration"]["z"].GetDouble();

				//velocity
				assert(compItr->HasMember("velocity"));
				assert((*compItr)["velocity"].IsObject());

				assert((*compItr)["velocity"].HasMember("x"));
				assert((*compItr)["velocity"]["x"].IsDouble());
				def.velocity.x = (float)(*compItr)["velocity"]["x"].GetDouble();

				assert((*compItr)["velocity"].HasMember("y"));
				assert((*compItr)["velocity"]["y"].IsDouble());
				def.velocity.y = (float)(*compItr)["velocity"]["y"].GetDouble();

				assert((*compItr)["velocity"].HasMember("z"));
				assert((*compItr)["velocity"]["z"].IsDouble());
				def.velocity.z = (float)(*compItr)["velocity"]["z"].GetDouble();

				assert(compItr->HasMember("shape"));
				assert((*compItr)["shape"].IsObject());

				assert((*compItr)["shape"].HasMember("type"));
				assert((*compItr)["shape"]["type"].IsString());
				std::string type = (*compItr)["shape"]["type"].GetString();
				if (type == "sphere")
				{
					def.shape_type = physics::interfaces::iShape::shape_types::sphere;

					assert((*compItr)["shape"].HasMember("radius"));
					assert((*compItr)["shape"]["radius"].IsDouble());
					def.radius = (*compItr)["shape"]["radius"].GetDouble();
				}
				else if (type == "plane")
				{
					def.shape_type = physics::interfaces::iShape::shape_types::plane;

					assert((*compItr)["shape"].HasMember("p1"));
					assert((*compItr)["shape"]["p1"].IsObject());

					assert((*compItr)["shape"]["p1"].HasMember("x"));
					assert((*compItr)["shape"]["p1"]["x"].IsDouble());
					def.p1.x = (*compItr)["shape"]["p1"]["x"].GetDouble();

					assert((*compItr)["shape"]["p1"].HasMember("y"));
					assert((*compItr)["shape"]["p1"]["y"].IsDouble());
					def.p1.y = (*compItr)["shape"]["p1"]["y"].GetDouble();

					assert((*compItr)["shape"]["p1"].HasMember("z"));
					assert((*compItr)["shape"]["p1"]["z"].IsDouble());
					def.p1.z = (*compItr)["shape"]["p1"]["z"].GetDouble();

					assert((*compItr)["shape"].HasMember("p2"));
					assert((*compItr)["shape"]["p2"].IsObject());

					assert((*compItr)["shape"]["p2"].HasMember("x"));
					assert((*compItr)["shape"]["p2"]["x"].IsDouble());
					def.p2.x = (*compItr)["shape"]["p2"]["x"].GetDouble();

					assert((*compItr)["shape"]["p2"].HasMember("y"));
					assert((*compItr)["shape"]["p2"]["y"].IsDouble());
					def.p2.y = (*compItr)["shape"]["p2"]["y"].GetDouble();

					assert((*compItr)["shape"]["p2"].HasMember("z"));
					assert((*compItr)["shape"]["p2"]["z"].IsDouble());
					def.p2.z = (*compItr)["shape"]["p2"]["z"].GetDouble();

					//
					assert((*compItr)["shape"].HasMember("p3"));
					assert((*compItr)["shape"]["p3"].IsObject());

					assert((*compItr)["shape"]["p3"].HasMember("x"));
					assert((*compItr)["shape"]["p3"]["x"].IsDouble());
					def.p3.x = (*compItr)["shape"]["p3"]["x"].GetDouble();

					assert((*compItr)["shape"]["p3"].HasMember("y"));
					assert((*compItr)["shape"]["p3"]["y"].IsDouble());
					def.p3.y = (*compItr)["shape"]["p3"]["y"].GetDouble();

					assert((*compItr)["shape"]["p3"].HasMember("z"));
					assert((*compItr)["shape"]["p3"]["z"].IsDouble());
					def.p3.z = (*compItr)["shape"]["p3"]["z"].GetDouble();
				}

				cEngine* engine = cEngine::getInstance();
				physics::interfaces::iPhysicsFactory* factory = engine->get_physics_factory();

				physics::interfaces::iRigidBody* body = factory->create_rigid_body(def);
				physics->configure_component(body);
				engine->get_world()->register_rigid_body(body);
				cSceneManager::getInstance()->getActiveScene()->addEntityToScene(entity);
			}
			break;
			}
		}
	}

	return true;
}

void cScene::setLightDebugSphere(cEntity* pSphere) {

	sLight* pCurLight = this->getSelectedLight();

	int program = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");

	//// Create debug object
	//cTransformComponent* pSphereTrans = static_cast<cTransformComponent*>(pSphere->components[1]);
	//cRenderMeshComponent* pSphereMesh = static_cast<cRenderMeshComponent*>(pSphere->components[3]);
	//pSphereTrans->set_position(glm::vec3(pCurLight->position));
	//pSphereMesh->objColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//pSphereTrans->scale = glm::vec3(0.1f);

	//glm::mat4 matBall = glm::mat4(1.0f);
	//cMeshRenderSystem::getInstance()->drawObject(pSphere, matBall, program, 1);

	//cLightHelper* pLightHelper = new cLightHelper();

	//// calculate 90% distance
	//float distance90Percent = pLightHelper->calcApproxDistFromAtten(0.90f, this->ACCURACY_OF_DISTANCE, this->INFINITE_DISTANCE,
	//	pCurLight->atten.x, pCurLight->atten.y, pCurLight->atten.z);
	//pSphereTrans->scale = glm::vec3(distance90Percent);
	//pSphereMesh->objColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//cMeshRenderSystem::getInstance()->drawObject(pSphere, matBall, program, 1);

	//// calculate 50% brightness
	//float distance50Percent = pLightHelper->calcApproxDistFromAtten(0.50f, this->ACCURACY_OF_DISTANCE, this->INFINITE_DISTANCE,
	//	pCurLight->atten.x, pCurLight->atten.y, pCurLight->atten.z);
	//pSphereTrans->scale = glm::vec3(distance50Percent);
	//pSphereMesh->objColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//cMeshRenderSystem::getInstance()->drawObject(pSphere, matBall, program, 1);

	//// calculate 25% brightness
	//float distance25Percent = pLightHelper->calcApproxDistFromAtten(0.25f, this->ACCURACY_OF_DISTANCE, this->INFINITE_DISTANCE,
	//	pCurLight->atten.x, pCurLight->atten.y, pCurLight->atten.z);
	//pSphereTrans->scale = glm::vec3(distance25Percent);
	//pSphereMesh->objColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//cMeshRenderSystem::getInstance()->drawObject(pSphere, matBall, program, 1);

	//// calculate 1% brightness
	//float distance1Percent = pLightHelper->calcApproxDistFromAtten(0.01f, this->ACCURACY_OF_DISTANCE, this->INFINITE_DISTANCE,
	//	pCurLight->atten.x, pCurLight->atten.y, pCurLight->atten.z);
	//pSphereTrans->scale = glm::vec3(distance1Percent);
	//pSphereMesh->objColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//cMeshRenderSystem::getInstance()->drawObject(pSphere, matBall, program, 1);

	return;
}

void cScene::update(float time)
{
	this->time_of_day += time;
	this->update_day_night_cycle();
	this->update_waves();
}

void cScene::update_day_night_cycle()
{
	float TOTAL_DAY_DURATION = 45;
	float PHASE_DURATION = TOTAL_DAY_DURATION / 5.0f;

	int current_phase = ((int)this->time_of_day % (int)TOTAL_DAY_DURATION) / PHASE_DURATION;
	float phase_time = fmod(this->time_of_day, PHASE_DURATION);
	switch (current_phase)
	{
	case 0: //night to dawn
		this->skybox_weights[0].y = phase_time / PHASE_DURATION;
		this->skybox_weights[0].x = 1.0f - this->skybox_weights[0].y;

		this->skybox_weights[0].z = 0.0f;
		this->skybox_weights[0].a = 0.0f;
		this->skybox_weights[1].x = 0.0f;
		break;

	case 1: //dusk to morning
		this->skybox_weights[0].x = 0.0f;

		this->skybox_weights[0].z = phase_time / PHASE_DURATION;
		this->skybox_weights[0].y = 1.0f - this->skybox_weights[0].z;

		this->skybox_weights[0].a = 0.0f;
		this->skybox_weights[1].x = 0.0f;
		break;

	case 2: //morning to day
		this->skybox_weights[0].x = 0.0f;
		this->skybox_weights[0].y = 0.0f;

		this->skybox_weights[0].a = phase_time / PHASE_DURATION;
		this->skybox_weights[0].z = 1.0f - this->skybox_weights[0].a;

		this->skybox_weights[1].x = 0.0f;
		break;

	case 3: //day to sunset
		this->skybox_weights[0].x = 0.0f;
		this->skybox_weights[0].y = 0.0f;
		this->skybox_weights[0].z = 0.0f;

		this->skybox_weights[1].x = phase_time / PHASE_DURATION;
		this->skybox_weights[0].a = 1.0f - this->skybox_weights[1].x;
		break;

	case 4: //sunset to night
		this->skybox_weights[0].y = 0.0f;
		this->skybox_weights[0].z = 0.0f;
		this->skybox_weights[0].a = 0.0f;

		this->skybox_weights[0].x = phase_time / PHASE_DURATION;
		this->skybox_weights[1].x = 1.0f - this->skybox_weights[0].x;
	default:
		break;
	}
}

void cScene::update_waves()
{
	this->wave_offset.x += 0.0005f;
	this->wave_offset.y += 0.0005f;
}
