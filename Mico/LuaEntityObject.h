//#pragma once
//#include "LuaCommon.h"
//#include "Entity.h"
//
//class LuaEntityObject
//{
//public:
//	// Constants
//	static const char className[];
//	static Luna<LuaEntityObject>::RegType methods[];
//
//	// Initialize the pointer
//	LuaEntityObject(lua_State *Lua);
//	~LuaEntityObject();
//	void setObject(lua_State *Lua);
//
//	// Methods we will need to use
//	int GetEntityId(lua_State *Lua);
//	
//	int GetPosition(lua_State *Lua);
//	int SetPosition(lua_State *Lua);
//
//	int GetScale(lua_State *Lua);
//	int SetScale(lua_State *Lua);
//
//	int GetOrientation(lua_State *Lua);
//	int SetOrientation(lua_State *Lua);
//
//	int Rotate(lua_State * Lua);
//
//
//
//private:
//	// The pointer to the 'real object' defined in object.cc
//	Entity* myEntity;
//};
//
