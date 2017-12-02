#include "LuaEntityObject.h"
#include "TransformationComponent.h"

LuaEntityObject::LuaEntityObject(lua_State * Lua)
{
	myEntity = (Entity*)lua_touserdata(Lua, 1);
}

LuaEntityObject::~LuaEntityObject()
{
}

void LuaEntityObject::setObject(lua_State * Lua)
{
	myEntity = (Entity*)lua_touserdata(Lua, 1);
}

unsigned int LuaEntityObject::GetEntityId(lua_State *Lua)
{
	lua_pushinteger(Lua, myEntity->GetEntityId());
	return 1;
}

int LuaEntityObject::GetTransforms(lua_State *L)
{
	TransformationComponent* transform = dynamic_cast<TransformationComponent*>(myEntity->GetComponent("TransformationComponent"));
	lua_pushlightuserdata(L, myEntity->GetComponent(componentName));
	return 1;
}

int LuaEntityObject::SetTransforms(lua_State * Lua)
{
	return 0;
}
