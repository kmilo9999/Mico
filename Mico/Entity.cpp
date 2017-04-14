#include "Entity.h"



Entity::Entity(vec3 position, quat orientation, vec3 scale ): 
	Position(position),Orientation(orientation),Scale(scale)
{
}


Entity::~Entity()
{
	if (boundingVolumen)
	{
		delete boundingVolumen;
	}
}

vec3 Entity::GetPosition()
{
	return Position;
}

void Entity::SetPosition(vec3 position)
{
	Position = position;
}

vec3 Entity::GetScale()
{
	return Scale;
}

void Entity::SetScale(vec3 scale)
{
	Scale = scale;
}

quat Entity::GetOrientation()
{
	return Orientation;
}

void Entity::SetOrientatio(quat orientation)
{
	Orientation = orientation;
}

void Entity::SetBoundingVolumen(BoundingVolumen * volumen)
{
	boundingVolumen = volumen;
}

BoundingVolumen * Entity::GetBoundingVolumen()
{
	return boundingVolumen;
}
