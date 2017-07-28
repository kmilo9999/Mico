#include "Entity.h"
#include "GraphicsComponent.h"


Entity::Entity( )
	: EntityId(0)
{
}


Entity::~Entity()
{

}

bool Entity::operator==(const Entity & rhs)
{
	return EntityId == rhs.EntityId;
}


void Entity::Initialize()
{
	ComponentsMap::iterator it = Components.begin();
	ComponentsMap::iterator endIt = Components.end();
	for (;it != endIt;++it)
	{
		it->second->Initialize(this);
	}
}

void Entity::Update()
{
	ComponentsMap::iterator it = Components.begin();
	ComponentsMap::iterator endIt = Components.end();
	for (;it != endIt;++it)
	{
		it->second->Update();
	}
}

void Entity::Draw(ShaderProgram & shaderProgram)
{
	GraphicsComponent* myGraphicsComponent
		= dynamic_cast<GraphicsComponent*>(GetComponent("GraphicsComponent"));
	myGraphicsComponent->Draw(shaderProgram);
}


unsigned int Entity::GetEntityId()
{
	return EntityId;
}

void Entity::SetEntityId(unsigned int id)
{
	EntityId = id;
}

void Entity::AddComponent(Component * component)
{
	Components.insert(std::make_pair(component->GetName(), component));
}

Component * Entity::GetComponent(string name)
{

	ComponentsMap::iterator it = Components.find(name);
	if (it != Components.end())
	{
		return it->second;
	}

	return nullptr;
}



