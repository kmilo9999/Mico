//#include "LuaEntityObject.h"
//#include "TransformationComponent.h"
//#include "Math.h"
//
//LuaEntityObject::LuaEntityObject(lua_State * Lua)
//{
//	myEntity = (Entity*)lua_touserdata(Lua, 1);
//}
//
//LuaEntityObject::~LuaEntityObject()
//{
//}
//
//void LuaEntityObject::setObject(lua_State * Lua)
//{
//	myEntity = (Entity*)lua_touserdata(Lua, 1);
//}
//
//int LuaEntityObject::GetEntityId(lua_State *Lua)
//{
//	lua_pushinteger(Lua, myEntity->GetEntityId());
//	return 1;
//}
//
//int LuaEntityObject::GetPosition(lua_State * Lua)
//{
//	TransformationComponent* transform = dynamic_cast<TransformationComponent*>(myEntity->GetComponent("TransformationComponent"));
//	lua_pushnumber(Lua, transform->GetPosition().x);
//	lua_pushnumber(Lua, transform->GetPosition().y);
//	lua_pushnumber(Lua, transform->GetPosition().z);
//	return 1;
//}
//
//int LuaEntityObject::SetPosition(lua_State * Lua)
//{
//	float x = (float)luaL_checknumber(Lua, 1);
//	float y = (float)luaL_checknumber(Lua, 2);
//	float z = (float)luaL_checknumber(Lua, 3);
//	vec3 position(x, y, z);
//	TransformationComponent* transform = dynamic_cast<TransformationComponent*>(myEntity->GetComponent("TransformationComponent"));
//	transform->SetPosition(position);
//	return 0;
//}
//
//int LuaEntityObject::GetScale(lua_State * Lua)
//{
//	TransformationComponent* transform = dynamic_cast<TransformationComponent*>(myEntity->GetComponent("TransformationComponent"));
//	lua_pushnumber(Lua, transform->GetScale().x);
//	lua_pushnumber(Lua, transform->GetScale().y);
//	lua_pushnumber(Lua, transform->GetScale().z);
//	return 1;
//}
//
//int LuaEntityObject::SetScale(lua_State * Lua)
//{
//	TransformationComponent* transform = dynamic_cast<TransformationComponent*>(myEntity->GetComponent("TransformationComponent"));
//	float x = (float)luaL_checknumber(Lua, 1);
//	float y = (float)luaL_checknumber(Lua, 2);
//	float z = (float)luaL_checknumber(Lua, 3);
//	vec3 position(x, y, z);
//	transform->SetScale(position);
//	return 0;
//}
//
//int LuaEntityObject::GetOrientation(lua_State * Lua)
//{
//	TransformationComponent* transform = dynamic_cast<TransformationComponent*>(myEntity->GetComponent("TransformationComponent"));
//	quat qOrientation = transform->GetOrientation();
//	vec3 eOrientation = Math::EulerAngles(qOrientation);
//
//	lua_pushnumber(Lua, eOrientation.x);
//	lua_pushnumber(Lua, eOrientation.y);
//	lua_pushnumber(Lua, eOrientation.z);
//
//	return 1;
//}
//
//int LuaEntityObject::SetOrientation(lua_State * Lua)
//{
//	TransformationComponent* transform = dynamic_cast<TransformationComponent*>(myEntity->GetComponent("TransformationComponent"));
//	vec3 eulerAngles((float)luaL_checknumber(Lua, 1), (float)luaL_checknumber(Lua, 2),
//		(float)luaL_checknumber(Lua, 3));
//
//	transform->SetOrientation(Math::fromEulerAngles(eulerAngles));
//	return 0;
//}
//
//int LuaEntityObject::Rotate(lua_State * Lua)
//{
//	TransformationComponent* transform = dynamic_cast<TransformationComponent*>(myEntity->GetComponent("TransformationComponent"));
//	float angle = (float)luaL_checknumber(Lua, 1);
//	float xAxis = (float)luaL_checknumber(Lua, 2);
//	float yAxis = (float)luaL_checknumber(Lua, 3);
//	float zAxis = (float)luaL_checknumber(Lua, 4);
//
//	quat rotation(angle, vec3(xAxis, yAxis, zAxis));
//	transform->Rotate(rotation);
//
//	return 0;
//}
