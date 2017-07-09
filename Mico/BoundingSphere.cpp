#include "BoundingSphere.h"
#include "Entity.h"
#include "TransformationComponent.h"
#include <iostream>

BoundingSphere::BoundingSphere( float radius)
	:radius(radius)
{
	ComponentName = "BoundingVolumen";
}


BoundingSphere::~BoundingSphere()
{
}

bool BoundingSphere::RayInstersection(vec3 start, vec3 rayDirection)
{
	TransformationComponent* transformation = 
		dynamic_cast<TransformationComponent*> (myOwner->GetComponent("TransformationComponent"));
	Position = transformation->GetPosition();

	float b = 2 * (rayDirection.x * (start.x - Position.x) +
		rayDirection.y * (start.y - Position.y) + rayDirection.z * (start.z - Position.z));
	float c = start.x * start.x - 2 * start.x * Position.x + Position.x * Position.x
		+ start.y * start.y - 2 * start.y * Position.y + Position.y * Position.y
		+ start.z * start.z - 2 * start.z * Position.z + Position.z * Position.z - radius * radius;

	float discr = (b * b - 4 * c);
	if (discr < 0)
	{
		selected = false;
		return false;
	}
	else
	{
		selected = true;
		return true;
	}
	
}

vec3 BoundingSphere::GetCenter()
{
	TransformationComponent* transformation =
		dynamic_cast<TransformationComponent*> (myOwner->GetComponent("TransformationComponent"));
	return transformation->GetPosition();
}

void BoundingSphere::SetCenter(vec3 n_center)
{
	Position = n_center;
}

float BoundingSphere::GetRadius()
{
	return radius;
}

void BoundingSphere::SetRadius(float n_radius)
{
	radius = n_radius;
}

void BoundingSphere::Update()
{
	/*TransformationComponent* transformationComponent =
		dynamic_cast<TransformationComponent*>(myOwner->GetComponent("TransformationComponent"));

	center = transformationComponent->GetPosition();*/
	
}

void BoundingSphere::Initialize(Entity* owner)
{
	myOwner = owner;
}

void BoundingSphere::ShutDown()
{
}
