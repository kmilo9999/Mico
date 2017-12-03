#pragma once
#include "lua/lua.hpp"
// I am using luna
#include "lua/luna.h"
// The header file for the real C++ object
#include "TransformationComponent.h"

class LuaTransformationComponentObject
{
public:
	LuaTransformationComponentObject(lua_State *Lua);
	~LuaTransformationComponentObject();

	void setObject(lua_State *Lua);

	// Methods we will need to use

	int GetPosition(lua_State *Lua);
	int SetPosition(lua_State *Lua);

	int GetScale(lua_State *Lua);
	int SetScale(lua_State *Lua);

	int GetOrientation(lua_State *Lua);
	int SetOrientation(lua_State *Lua);

	int rotate(lua_State * Lua);


private:
	// The pointer to the 'real object' defined in object.cc
	TransformationComponent* myTransformation;
};

