#pragma once
#include "TexturedModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <map>
#include "BoundingSphere.h"
#include "Component.h"


using namespace glm;
class EntityManager;
class Entity
{
	friend class EntityManager;

   public:
	
    typedef map<std::string, Component*> ComponentsMap;

	~Entity();

	bool operator==(const Entity& rhs);

	
	virtual unsigned int GetEntityId();
	virtual void SetEntityId(unsigned int id);
	virtual void AddComponent(Component* component);
	virtual Component* GetComponent(string name);
	virtual void Initialize();

	void Update();

	void Draw(ShaderProgram & shaderProgram);

protected:
	ComponentsMap Components;
	Entity();
	
	unsigned int EntityId;
	

};

