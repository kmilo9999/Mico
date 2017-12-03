//#pragma once
//
//#include "Component.h"
//#include "LuaCommon.h"
//
//
//
//class ScriptComponent:public Component
//{
//public:
//	ScriptComponent(lua_State* luaState);
//	~ScriptComponent();
//	virtual void Update();
//
//	virtual void Initialize(Entity*);
//	virtual void ShutDown();
//
//	const std::string& scriptName();
//	void setScriptName(const std::string& scriptName);
//
//	void setLuaState(lua_State* lua_state);
//
//private:
//	std::string myScriptName;
//	lua_State* myLuaState;
//	
//};
//
