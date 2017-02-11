#include "Entity.h"



Entity::Entity(vec3 position, quat orientation, vec3 scale ): 
	Position(position),Orientation(orientation),Scale(scale)
{
}


Entity::~Entity()
{
}

vec3 Entity::GetPosition()
{
	return vec3();
}

void Entity::SetPosition(vec3 position)
{
}

vec3 Entity::GetScale()
{
	return vec3();
}

void Entity::SetScale(vec3 scale)
{
}

quat Entity::GetOrientation()
{
	return quat();
}

void Entity::SetOrientatio(quat orientation)
{
}
