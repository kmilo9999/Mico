#pragma once
#include <string>


class Entity;
class Component
{
public:
	
	virtual ~Component() {};
	virtual void Update() = 0;
	std::string GetName() { return ComponentName; };

	Entity* GetOwner() { return myOwner; };

	virtual void Initialize(Entity*) = 0;
	virtual void ShutDown() = 0 ;


protected:
	Entity* myOwner;
	std::string ComponentName;
};

