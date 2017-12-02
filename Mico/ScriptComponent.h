#pragma once

#include "Component.h"
#include "lua/lua.hpp"

class ScriptComponent:public Component
{
public:
	ScriptComponent();
	~ScriptComponent();
	virtual void Update();

	virtual void Initialize(Entity*);
	virtual void ShutDown();

};

