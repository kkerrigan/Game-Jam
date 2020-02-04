#ifndef _cLuaBrain_HG_
#define _cLuaBrain_HG_

// Lua includes
extern "C" {
#include <Lua5.3.3/lua.h>
#include <Lua5.3.3/lauxlib.h>
#include <Lua5.3.3/lualib.h>
}

#include <map>
#include <vector>
#include <string>

#include "cEntity.h"

class cLuaBrain {
public:
	// Singleton setup
	static cLuaBrain* getInstance();
	cLuaBrain(cLuaBrain const&) = delete;
	void operator=(cLuaBrain const&) = delete;

	void runScript(std::string scriptName);
	void loadScript(std::string scriptName, std::string scriptSource);
	void deleteScript(std::string scriptName);

	void update(float deltaTime); // Call all active scripts that are loaded

private:
	cLuaBrain(); // Init Lua and set callback functions
	~cLuaBrain();

	std::map<std::string /*scriptName*/, std::string /*scriptSource*/> m_mapScripts;
	static std::vector<cEntity*>* m_entities;
	lua_State* m_luaState;

	std::string m_decodeLuaErrorToString(int error);
};

#endif // !_cLuaBrain_HG_

