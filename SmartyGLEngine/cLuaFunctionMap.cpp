#include "cCommandGroup.h"
#include "cSceneManager.h"
#include "cOrientToEntity.h"
#include "cMoveToTime.h"
#include "cFollowEntity.h"
#include "cFadeEntityColorTrigger.h"
#include "cFollowCurve.h"
#include "cMoveToDistance.h"
#include "cCameraFollowEntity.h"
#include "cCameraMoveToTime.h"
#include "cCameraMoveToDistance.h"
#include "cOrientToEntityDistance.h"

// Lua includes
extern "C" {
#include <Lua5.3.3/lua.h>
#include <Lua5.3.3/lauxlib.h>
#include <Lua5.3.3/lualib.h>
}

#include <iostream>
#include <map>

std::map<std::string, cCommandGroup*> commandGroups;
std::map<std::string, iCommand*> commands;

int createCommandGroup(lua_State* luaState)
{
	const char* name = lua_tostring(luaState, 1);
	bool isParallel = lua_toboolean(luaState, 2);

	commandGroups[name] = new cCommandGroup(name, isParallel);

	std::cout << "Command group created: " << std::string(name) << std::endl;

	return 0;
}

int addCommandGroupToGroup(lua_State* luaState)
{
	const char* groupToAdd = lua_tostring(luaState, 1);
	const char* destinationGroup = lua_tostring(luaState, 2);

	commandGroups[destinationGroup]->commandGroups.push_back(commandGroups[groupToAdd]);

	std::cout << "Command group " << std::string(groupToAdd) << " added to command group " << std::string(destinationGroup) << std::endl;

	return 0;
}

int orientToEntity(lua_State* luaState)
{
	const char* name = lua_tostring(luaState, 1);
	int entityToRotate = lua_tointeger(luaState, 2);
	int lookAtEntity = lua_tointeger(luaState, 3);
	float speed = lua_tonumber(luaState, 4);
	float easeIn = lua_tonumber(luaState, 5);
	float easeOut = lua_tonumber(luaState, 6);

	cEntity* entityA = cSceneManager::getInstance()->getActiveScene()->findObjectById(entityToRotate);
	cEntity* entityB = cSceneManager::getInstance()->getActiveScene()->findObjectById(lookAtEntity);

	cOrientToEntity* orientCommand = new cOrientToEntity(entityA, entityB, speed, easeIn, easeOut);
	std::cout << "orientCommand for entity " << entityToRotate << " created." << std::endl;

	commandGroups[name]->commands.push_back(orientCommand);


	return 0;
}

int moveToTime(lua_State* luaState)
{
	const char* name = lua_tostring(luaState, 1);
	int entityId = lua_tointeger(luaState, 2);
	float x = lua_tonumber(luaState, 3);
	float y = lua_tonumber(luaState, 4);
	float z = lua_tonumber(luaState, 5);
	float time = lua_tonumber(luaState, 6);
	float easeIn = lua_tonumber(luaState, 7);
	float easeOut = lua_tonumber(luaState, 8);

	cEntity* entityA = cSceneManager::getInstance()->getActiveScene()->findObjectById(entityId);

	cMoveToTime* moveCommand = new cMoveToTime(entityA, glm::vec3(x, y, z), time, easeIn, easeOut);
	std::cout << "moveToTime command for entity " << entityId << " created." << std::endl;

	commandGroups[name]->commands.push_back(moveCommand);


	return 0;
}

int followEntity(lua_State* luaState)
{
	const char* name = lua_tostring(luaState, 1);
	int entityFollower = lua_tointeger(luaState, 2);
	int entityTarget = lua_tointeger(luaState, 3);

	float followX = lua_tonumber(luaState, 4);
	float followY = lua_tonumber(luaState, 5);
	float followZ = lua_tonumber(luaState, 6);

	float minDistance = lua_tonumber(luaState, 7);
	float maxSpeedDistance = lua_tonumber(luaState, 8);
	float maxSpeed = lua_tonumber(luaState, 9);

	float offsetX = lua_tonumber(luaState, 10);
	float offsetY = lua_tonumber(luaState, 11);
	float offsetZ = lua_tonumber(luaState, 12);

	float easeIn = lua_tonumber(luaState, 13);
	float easeOut = lua_tonumber(luaState, 14);

	cEntity* entityA = cSceneManager::getInstance()->getActiveScene()->findObjectById(entityFollower);
	cEntity* entityB = cSceneManager::getInstance()->getActiveScene()->findObjectById(entityTarget);

	cFollowEntity* moveCommand = new cFollowEntity(entityA, entityB, glm::vec3(followX, followY, followZ), minDistance, maxSpeedDistance, maxSpeed, glm::vec3(offsetX, offsetY, offsetZ), easeIn, easeOut);
	std::cout << "entity " << entityFollower << " is following entity " << entityTarget << std::endl;

	commandGroups[name]->commands.push_back(moveCommand);


	return 0;
}

int addTrigger(lua_State* luaState)
{
	const char* name = lua_tostring(luaState, 1);
	float entityId = lua_tonumber(luaState, 2);

	float r = lua_tonumber(luaState, 3);
	float g = lua_tonumber(luaState, 4);
	float b = lua_tonumber(luaState, 5);

	float fadeDuration = lua_tonumber(luaState, 6);
	float radius = lua_tonumber(luaState, 7);

	std::string commandToExecute = lua_tostring(luaState, 8);

	if (commandToExecute == std::string("cFadeEntityColorTrigger"))
	{
		cFadeEntityColorTrigger* trigger = new cFadeEntityColorTrigger(cSceneManager::getInstance()->getActiveScene()->findObjectById(entityId), glm::vec4(r, g, b, 1.0f), fadeDuration, radius);
		commandGroups[name]->commands.push_back(trigger);
		std::cout << "cFadeEntityColorTrigger trigger created on entity " << entityId << std::endl;
	}

	return 0;
}

int entityFollowCurve(lua_State* luaState)
{
	const char* name = lua_tostring(luaState, 1);
	float entityId = lua_tonumber(luaState, 2);

	float destinationX = lua_tonumber(luaState, 3);
	float destinationY = lua_tonumber(luaState, 4);
	float destinationZ = lua_tonumber(luaState, 5);
	glm::vec3 destination(destinationX, destinationY, destinationZ);

	float time = lua_tonumber(luaState, 6);

	float controlPointX = lua_tonumber(luaState, 7);
	float controlPointY = lua_tonumber(luaState, 8);
	float controlPointZ = lua_tonumber(luaState, 9);
	glm::vec3 controlPoint(controlPointX, controlPointY, controlPointZ);

	float easeIn = lua_tonumber(luaState, 10);
	float easeOut = lua_tonumber(luaState, 11);

	cEntity* entity = cSceneManager::getInstance()->getActiveScene()->findObjectById(entityId);

	cFollowCurve* curve = new cFollowCurve(entity, destination, time, controlPoint, easeIn, easeOut);
	commandGroups[name]->commands.push_back(curve);
	return 0;
}

int moveToDistance(lua_State* luaState)
{
	const char* name = lua_tostring(luaState, 1);
	int entityId = lua_tointeger(luaState, 2);
	float x = lua_tonumber(luaState, 3);
	float y = lua_tonumber(luaState, 4);
	float z = lua_tonumber(luaState, 5);
	float time = lua_tonumber(luaState, 6);
	float easeIn = lua_tonumber(luaState, 7);
	float easeOut = lua_tonumber(luaState, 8);

	cEntity* entityA = cSceneManager::getInstance()->getActiveScene()->findObjectById(entityId);

	cMoveToDistance* moveCommand = new cMoveToDistance(entityA, glm::vec3(x, y, z), time, easeIn, easeOut);
	std::cout << "moveToDistance command for entity " << entityId << " created." << std::endl;

	commandGroups[name]->commands.push_back(moveCommand);

	return 0;
}

int cameraFollowEntity(lua_State* luaState)
{
	const char* name = lua_tostring(luaState, 1);
	int entityTarget = lua_tointeger(luaState, 2);

	float followX = lua_tonumber(luaState, 3);
	float followY = lua_tonumber(luaState, 4);
	float followZ = lua_tonumber(luaState, 5);

	float minDistance = lua_tonumber(luaState, 6);
	float maxSpeedDistance = lua_tonumber(luaState, 7);
	float maxSpeed = lua_tonumber(luaState, 8);

	float offsetX = lua_tonumber(luaState, 9);
	float offsetY = lua_tonumber(luaState, 10);
	float offsetZ = lua_tonumber(luaState, 11);

	float easeIn = lua_tonumber(luaState, 12);
	float easeOut = lua_tonumber(luaState, 13);

	float time = lua_tonumber(luaState, 14);

	cEntity* entityA = cSceneManager::getInstance()->getActiveScene()->findObjectById(entityTarget);
	cFlyCamera* camera = &cSceneManager::getInstance()->getActiveScene()->flyCamera;

	cCameraFollowEntity* moveCommand = new cCameraFollowEntity(camera, entityA, glm::vec3(followX, followY, followZ), minDistance, maxSpeedDistance, maxSpeed, glm::vec3(offsetX, offsetY, offsetZ), easeIn, easeOut, time);
	std::cout << "camera is following entity " << entityTarget << std::endl;

	commandGroups[name]->commands.push_back(moveCommand);


	return 0;
}

int cameraMoveToTime(lua_State* luaState)
{
	const char* name = lua_tostring(luaState, 1);
	float x = lua_tonumber(luaState, 2);
	float y = lua_tonumber(luaState, 3);
	float z = lua_tonumber(luaState, 4);
	float time = lua_tonumber(luaState, 5);
	float easeIn = lua_tonumber(luaState, 6);
	float easeOut = lua_tonumber(luaState, 7);

	cFlyCamera* camera = &cSceneManager::getInstance()->getActiveScene()->flyCamera;

	cCameraMoveToTime* moveCommand = new cCameraMoveToTime(camera, glm::vec3(x, y, z), time, easeIn, easeOut);
	std::cout << "cameraMoveToTime command created" << std::endl;

	commandGroups[name]->commands.push_back(moveCommand);


	return 0;
}

int cameraMoveToDistance(lua_State* luaState)
{
	const char* name = lua_tostring(luaState, 1);
	float x = lua_tonumber(luaState, 2);
	float y = lua_tonumber(luaState, 3);
	float z = lua_tonumber(luaState, 4);
	float time = lua_tonumber(luaState, 5);
	float easeIn = lua_tonumber(luaState, 6);
	float easeOut = lua_tonumber(luaState, 7);

	cFlyCamera* camera = &cSceneManager::getInstance()->getActiveScene()->flyCamera;

	cCameraMoveToDistance* moveCommand = new cCameraMoveToDistance(camera, glm::vec3(x, y, z), time, easeIn, easeOut);
	std::cout << "moveToDistance command for camera created." << std::endl;

	commandGroups[name]->commands.push_back(moveCommand);

	return 0;
}

int orientToEntityDistance(lua_State* luaState)
{
	const char* name = lua_tostring(luaState, 1);
	int entityToRotate = lua_tointeger(luaState, 2);
	int lookAtEntity = lua_tointeger(luaState, 3);
	float speed = lua_tonumber(luaState, 4);
	float easeIn = lua_tonumber(luaState, 5);
	float easeOut = lua_tonumber(luaState, 6);

	cEntity* entityA = cSceneManager::getInstance()->getActiveScene()->findObjectById(entityToRotate);
	cEntity* entityB = cSceneManager::getInstance()->getActiveScene()->findObjectById(lookAtEntity);

	cOrientToEntityDistance* orientCommand = new cOrientToEntityDistance(entityA, entityB, speed, easeIn, easeOut);
	std::cout << "orientCommand for entity " << entityToRotate << " created." << std::endl;

	commandGroups[name]->commands.push_back(orientCommand);

	return 0;
}