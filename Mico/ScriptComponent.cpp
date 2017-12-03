//#include "ScriptComponent.h"
//
//
//ScriptComponent::ScriptComponent(lua_State * luaState):myLuaState(luaState)
//{
//}
//
//ScriptComponent::~ScriptComponent()
//{
//}
//
//void ScriptComponent::Update()
//{
//	luaL_dofile(myLuaState, myScriptName.c_str());
//}
//
//void ScriptComponent::Initialize(Entity * owner)
//{
//	myOwner = owner;
//}
//
//void ScriptComponent::ShutDown()
//{
//}
//
//const std::string & ScriptComponent::scriptName()
//{
//	// TODO: insert return statement here
//	return myScriptName;
//}
//
//void ScriptComponent::setScriptName(const std::string & scriptName)
//{
//	myScriptName = "scripts/" + scriptName;
//}
//
//void ScriptComponent::setLuaState(lua_State * lua_state)
//{
//	myLuaState = lua_state;
//}
