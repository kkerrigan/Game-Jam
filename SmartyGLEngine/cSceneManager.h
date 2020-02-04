#ifndef _cSceneManager_HG_
#define _cSceneManager_HG_

#include <map>
#include <string>

#include "cScene.h"

class cSceneManager {
public:
	static cSceneManager* getInstance();
	cSceneManager(cSceneManager const&) = delete;
	void operator=(cSceneManager const&) = delete;

	cScene* createScene(std::string name);
	
	void setActiveScene(std::string name);
	cScene* getActiveScene();

	bool serializeActiveScene() {

	}

private:
	std::string activeScene;
	std::map<std::string, cScene*> scenes;

	cSceneManager();
	~cSceneManager();

};

#endif
