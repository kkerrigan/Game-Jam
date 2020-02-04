#include "cLuaBrain.h"
#include "cTransformComponent.h"
#include "cEulerMovementComponent.h"
#include <sstream>
#include <iostream>

int createCommandGroup(lua_State* luaState);
int addCommandGroupToGroup(lua_State* luaState);
int orientToEntity(lua_State* luaState);
int moveToTime(lua_State* luaState);
int followEntity(lua_State* luaState);
int addTrigger(lua_State* luaState);
int entityFollowCurve(lua_State* luaState);
int moveToDistance(lua_State* luaState);
int cameraFollowEntity(lua_State* luaState);
int cameraMoveToTime(lua_State* luaState);
int cameraMoveToDistance(lua_State* lusState);
int orientToEntityDistance(lua_State* luaState);

std::vector<cEntity*>* cLuaBrain::m_entities;

cLuaBrain::cLuaBrain() {

	this->m_entities = nullptr;

	// Create new Lua state
	// NOTE: this is common to ALL scripts in this case.
	this->m_luaState = luaL_newstate();

	luaL_openlibs(this->m_luaState);

	// TODO: Push all C functions to Lua
	lua_pushcfunction(this->m_luaState, createCommandGroup);
	lua_setglobal(this->m_luaState, "createCommandGroup");

	lua_pushcfunction(this->m_luaState, addCommandGroupToGroup);
	lua_setglobal(this->m_luaState, "addCommandGroupToGroup");

	lua_pushcfunction(this->m_luaState, orientToEntity);
	lua_setglobal(this->m_luaState, "orientToEntity");

	lua_pushcfunction(this->m_luaState, moveToTime);
	lua_setglobal(this->m_luaState, "moveToTime");

	lua_pushcfunction(this->m_luaState, followEntity);
	lua_setglobal(this->m_luaState, "followEntity");

	lua_pushcfunction(this->m_luaState, addTrigger);
	lua_setglobal(this->m_luaState, "addTrigger");

	lua_pushcfunction(this->m_luaState, entityFollowCurve);
	lua_setglobal(this->m_luaState, "entityFollowCurve");

	lua_pushcfunction(this->m_luaState, moveToDistance);
	lua_setglobal(this->m_luaState, "moveToDistance");

	lua_pushcfunction(this->m_luaState, cameraFollowEntity);
	lua_setglobal(this->m_luaState, "cameraFollowEntity");

	lua_pushcfunction(this->m_luaState, cameraMoveToTime);
	lua_setglobal(this->m_luaState, "cameraMoveToTime");

	lua_pushcfunction(this->m_luaState, cameraMoveToDistance);
	lua_setglobal(this->m_luaState, "cameraMoveToDistance");
	
	lua_pushcfunction(this->m_luaState, orientToEntityDistance);
	lua_setglobal(this->m_luaState, "orientToEntityDistance");

	return;
}

cLuaBrain::~cLuaBrain() {

	lua_close(this->m_luaState);

	return;
}

cLuaBrain* cLuaBrain::getInstance() {
	static cLuaBrain instance;

	return &instance;
}

// Saves (and overwrites) any script
// scriptName is just so we can delete them later
void cLuaBrain::loadScript(std::string scriptName, std::string scriptSource) {

	this->m_mapScripts[scriptName] = scriptSource;

	return;
}

void cLuaBrain::deleteScript(std::string scriptName) {

	// TODO: Delete this script
	return;
}

void cLuaBrain::runScript(std::string scriptName) {

	int error = luaL_loadstring(this->m_luaState, this->m_mapScripts[scriptName].c_str());
	if (error != 0) { // 0 = No error

		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << this->m_decodeLuaErrorToString(error) << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;
	}

	// execute function in "protected" mode, where problems are 
	// caught and placed on the stack for investigation
	error = lua_pcall(this->m_luaState, /* lua state*/
					  0, /*nargs: number of arguments pushed onto the Lua stack*/
		              0, /*nresults: number of results that should be on the stack at the end*/
		              0);/*errfunc: location, in stack, of error function.
						 if 0, results are on the top of the stack*/

	if (error != 0) {
		std::cout << "Lua: there was an error..." << std::endl;
		std::cout << this->m_decodeLuaErrorToString(error) << std::endl;

		std::string luaError;
		// Get error information from to of stack (-1 is top)
		luaError.append(lua_tostring(this->m_luaState, -1));

		// Make error message a little more clear
		std::cout << "-------------------------------------------------------" << std::endl;
		std::cout << "Error running Lua script: ";
		std::cout << luaError << std::endl;
		std::cout << "-------------------------------------------------------" << std::endl;

		// We passed zero as errfunc, so no error is on stack
		// Pop error message from the stack
		lua_pop(this->m_luaState, 1);
	}

	return;
}

void cLuaBrain::update(float deltaTime) {

	return;

	for (std::map<std::string, std::string>::iterator itScript = this->m_mapScripts.begin(); itScript != this->m_mapScripts.end(); ++itScript) {

		// Pass the script into Lua and update
		std::string curScript = itScript->second;

		int error = luaL_loadstring(this->m_luaState, curScript.c_str());
		if (error != 0) {
			
			std::cout << "-------------------------------------------------------" << std::endl;
			std::cout << "Error running Lua script: ";
			std::cout << itScript->first << std::endl;
			std::cout << this->m_decodeLuaErrorToString(error) << std::endl;
			std::cout << "-------------------------------------------------------" << std::endl;
			
			continue;
		}

		error = lua_pcall(this->m_luaState, 0, 0, 0); // See runScript for parameter descriptions.
		if (error != 0) {

			std::cout << "Lua: There was an error..." << std::endl;
			std::cout << this->m_decodeLuaErrorToString(error) << std::endl;

			std::string luaError;
			
			// Get error information from top of stack (-1 is top)
			luaError.append(lua_tostring(this->m_luaState, -1));

			// Make error message a little more clear
			std::cout << "-------------------------------------------------------" << std::endl;
			std::cout << "Error running Lua script: ";
			std::cout << itScript->first << std::endl;
			std::cout << luaError << std::endl;
			std::cout << "-------------------------------------------------------" << std::endl;
			// We passed zero (0) as errfunc, so error is on stack)
			lua_pop(this->m_luaState, 1);  /* pop error message from the stack */

			continue;
		}
	}

	// TODO: Lots of Lua stuff here...

	return;
}


std::string cLuaBrain::m_decodeLuaErrorToString(int error) {

	switch (error) {
	case 0:
		return "Lua: no error";
		break;
	case LUA_ERRSYNTAX:
		return "Lua: syntax error";
		break;
	case LUA_ERRMEM:
		return "Lua: memory allocation error";
		break;
	case LUA_ERRRUN:
		return "Lua: Runtime error";
		break;
	case LUA_ERRERR:
		return "Lua: Error while running the error handler function";
		break;
	default:
		return "Lua: UNKNOWN error";
		break;
	}
}