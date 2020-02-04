/*
* Mediator for the scene
*/

#ifndef _cScene_HG_
#define _cScene_HG_

#include <glm/vec3.hpp>
#include <vector>
#include <string>

#include "cEntity.h"
#include "sLight.h"
#include "iInputCommand.h"
#include "cFlyCamera.h"

#include "cEntityBuilder.h"
#include "cLightManager.h"
#include "cSoundManager.h"
#include "c3dSoundComponent.h"
#include "cMazeMaker.h"
#include "cPathFinding.h"

class cScene {
public:
	cScene();
	~cScene();

	std::map<unsigned long long, cMazeMaker::maze_cube_info> maze_info;

	glm::vec3 find_corridor_position();

	cPathFinding* path_finding;

	int selectedEntity;
	int selectedLight;
	int selectedChannelGroup;

	std::string sceneName;

	cFlyCamera flyCamera;
	bool isCursorInWindow;

	//TODO: remove old camera
	glm::vec3 cameraPos;
	glm::vec3 cameraLookAt;
	glm::vec3 cameraUp;

	// Scene modifiers
	void setSceneName(std::string name);
	void updateWindowTitle(GLFWwindow* window);
	bool getIsScenePaused();
	void setIsScenePaused(bool val);
	void handleCommand(iInputCommand* command);

	// Scene cameras
	void setCameraPosition(glm::vec3 pos);
	void setCameraLookAt(glm::vec3 lookAt);

	void build_maze();

	// Scene entities
	void addEntityToScene(cEntity* entity);
	void removeEntityFromScene(cEntity* entiy);
	cEntity* getSelectedEntity();
	cEntity* findObjectByFriendlyName(std::string theNameToFind);
	cEntity* findObjectById(int id);
	size_t getNumberOfEntites();
	std::vector<cEntity*>& getEntities();

	// Scene lights
	void addLightToScene(sLight* light);
	void removeLightFromScene(sLight* light);
	sLight* getSelectedLight();
	size_t getNumberOfLights();
	std::vector<sLight*> getLights();

	// Scene non-3D sounds
	void addSoundToScene(cSoundManager::sSoundInfo* soundInfo);
	cSoundManager::sSoundInfo* getSoundInfo(unsigned index);
	std::vector<cSoundManager::sSoundInfo*> getSounds();

	void cycle_entities(bool forward = true);

	// Save/Load
	bool serialize();
	bool deserialize(std::string filename);

	// Debug Functions
	bool bIsLightDebug;
	bool bIsRenderDebug;
	void setLightDebugSphere(cEntity* pSphere);
	bool bDisplayAABBs;

	bool show_physics_debug;
	bool show_camera_debug;

	bool follow_cam_enabled = true;

	cMazeMaker theMM;

	void update(float time);
	void update_day_night_cycle();

	float time_of_day;
	glm::vec4 skybox_weights[2] = {glm::vec4(0.0f)};

	glm::vec2 wave_offset;
	void update_waves();

private:
	std::vector<cEntity*> entities;
	std::vector<sLight*> lights;
	std::vector<cSoundManager::sSoundInfo*> sounds;
	bool isScenePaused;

	// Debug light constants
	const float ACCURACY_OF_DISTANCE = 0.01f;
	const float INFINITE_DISTANCE = 1000.0f;
};

#endif
