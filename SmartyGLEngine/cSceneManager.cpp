#include "cSceneManager.h"



cSceneManager::cSceneManager()
{
}


cSceneManager::~cSceneManager()
{
}

cSceneManager* cSceneManager::getInstance() {
	static cSceneManager instance;

	return &instance;
}

cScene* cSceneManager::createScene(std::string name) {
	this->scenes[name] = new cScene();
	this->scenes[name]->setSceneName(name);
	return this->scenes[name];
}

void cSceneManager::setActiveScene(std::string name) {
	this->activeScene = name;
}

cScene* cSceneManager::getActiveScene() {
	return this->scenes[this->activeScene];
}