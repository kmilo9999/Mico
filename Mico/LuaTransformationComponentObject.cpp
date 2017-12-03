#include "LuaTransformationComponentObject.h"
#include "Math.h"

LuaTransformationComponentObject::LuaTransformationComponentObject(lua_State * Lua)
{
	myTransformation = (TransformationComponent*)lua_touserdata(Lua, 1);
}

LuaTransformationComponentObject::~LuaTransformationComponentObject()
{
}

void LuaTransformationComponentObject::setObject(lua_State * Lua)
{
	myTransformation = (TransformationComponent*)lua_touserdata(Lua, 1);
}

int LuaTransformationComponentObject::GetPosition(lua_State * Lua)
{
	lua_pushnumber(Lua, myTransformation->GetPosition().x);
	lua_pushnumber(Lua, myTransformation->GetPosition().y);
	lua_pushnumber(Lua, myTransformation->GetPosition().z);
	return 1;
}

int LuaTransformationComponentObject::SetPosition(lua_State * Lua)
{
	float x = (float)luaL_checknumber(Lua, 1);
	float y = (float)luaL_checknumber(Lua, 2);
	float z = (float)luaL_checknumber(Lua, 3);
	vec3 position(x, y, z);
	myTransformation->SetPosition(position);
	return 0;
}

int LuaTransformationComponentObject::GetScale(lua_State * Lua)
{
	lua_pushnumber(Lua, myTransformation->GetScale().x);
	lua_pushnumber(Lua, myTransformation->GetScale().y);
	lua_pushnumber(Lua, myTransformation->GetScale().z);
	return 1;
}

int LuaTransformationComponentObject::SetScale(lua_State * Lua)
{
	float x = (float)luaL_checknumber(Lua, 1);
	float y = (float)luaL_checknumber(Lua, 2);
	float z = (float)luaL_checknumber(Lua, 3);
	vec3 position(x, y, z);
	myTransformation->SetScale(position);
	return 0;
}

int LuaTransformationComponentObject::GetOrientation(lua_State * Lua)
{	
	quat qOrientation = myTransformation->GetOrientation();
	vec3 eOrientation = Math::EulerAngles(qOrientation);
		
	lua_pushnumber(Lua, eOrientation.x);
	lua_pushnumber(Lua, eOrientation.y);
	lua_pushnumber(Lua, eOrientation.z);

	return 1;
}

int LuaTransformationComponentObject::SetOrientation(lua_State * Lua)
{
	vec3 eulerAngles((float)luaL_checknumber(Lua, 1), (float)luaL_checknumber(Lua, 2),
		(float)luaL_checknumber(Lua, 3));

	myTransformation->SetOrientation(Math::fromEulerAngles(eulerAngles));
    return 0;
}

int LuaTransformationComponentObject::rotate(lua_State * Lua)
{
	float angle = (float)luaL_checknumber(Lua, 1);
	float xAxis = (float)luaL_checknumber(Lua, 2);
	float yAxis = (float)luaL_checknumber(Lua, 3);
	float zAxis = (float)luaL_checknumber(Lua, 3);
	
	quat rotation(angle, vec3(xAxis, yAxis, zAxis));
	myTransformation->Rotate(rotation);

	return 0;
}
