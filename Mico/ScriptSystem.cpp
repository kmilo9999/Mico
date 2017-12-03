//#include "ScriptSystem.h"
//#include "LuaEntityObject.h"
//
//ScriptSystem * SCRIPTSYSTEM = nullptr;
//
//// Define the Lua ClassName
//const char LuaEntityObject::className[] = "LuaGameObject";
//
//// Define the methods we will expose to Lua
//// Check luaobject.h for the definitions...
//#define method(class, name) {#name, &class::name}
//Luna<LuaEntityObject>::RegType LuaEntityObject::methods[] = {
//	method(LuaEntityObject, GetEntityId),
//	method(LuaEntityObject, GetPosition),
//	method(LuaEntityObject, SetPosition),
//	method(LuaEntityObject, GetScale),
//	method(LuaEntityObject, SetScale),
//	method(LuaEntityObject, GetOrientation),
//	method(LuaEntityObject, SetOrientation),
//	method(LuaEntityObject, Rotate),
//	{ 0,0 }
//};
//
//ScriptSystem::ScriptSystem()
//{
//}
//
//
//ScriptSystem::~ScriptSystem()
//{
//	lua_close(myLua);
//}
//
//void ScriptSystem::Init()
//{
//	// Init Lua
//	myLua = luaL_newstate();
//	luaopen_base(myLua);
//	luaopen_table(myLua);
//	luaopen_io(myLua);
//	luaopen_string(myLua);
//	luaopen_math(myLua);
//	luaopen_debug(myLua);
//	SCRIPTSYSTEM = this;
//		
//
//	// Register the LuaGameObject data type with Lua
//	Luna<LuaEntityObject>::Register(myLua);
//}
//
//void ScriptSystem::Update()
//{
//}
//
//std::string ScriptSystem::className()
//{
//	return "MICO::SCRIPTSYSTEM";
//}
//
//lua_State * ScriptSystem::luaState()
//{
//	return myLua;
//}
