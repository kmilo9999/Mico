#pragma once
#include "lua/lua.hpp"
// I am using luna
#include "lua/luna.h"
// The header file for the real C++ object
#include "Entity.h"

class LuaEntityObject
{
public:
	// Constants
	static const char className[];
	static Luna<LuaEntityObject>::RegType methods[];

	// Initialize the pointer
	LuaEntityObject(lua_State *Lua);
	~LuaEntityObject();
	void setObject(lua_State *Lua);

	// Methods we will need to use
	unsigned int GetEntityId(lua_State *Lua);
	int GetTransforms(lua_State *Lua);
	int SetTransforms(lua_State *Lua);
	


private:
	// The pointer to the 'real object' defined in object.cc
	Entity* myEntity;
};

