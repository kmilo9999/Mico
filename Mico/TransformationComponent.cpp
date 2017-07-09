#include "TransformationComponent.h"



TransformationComponent::TransformationComponent(vec3 position, 
	quat orientation, vec3 scale)
	: Position(position)
	, Orientation(orientation)
	, Scale(scale)
{
	ComponentName = "TransformationComponent";
}


TransformationComponent::~TransformationComponent()
{
}

void TransformationComponent::Initialize(Entity * owner)
{
	myOwner = owner;
}

void TransformationComponent::ShutDown()
{
}

void TransformationComponent::Update()
{

}

vec3 TransformationComponent::GetPosition()
{
	return Position;
}

void TransformationComponent::SetPosition(vec3 position)
{
	Position = position;
	/*if (boundingVolumen)
	{
		boundingVolumen->SetPotision(Position);
	}*/
}

vec3 TransformationComponent::GetScale()
{
	return Scale;
}

void TransformationComponent::SetScale(vec3 scale)
{
	Scale = scale;
}

quat TransformationComponent::GetOrientation()
{
	return Orientation;
}

void TransformationComponent::SetOrientatio(quat orientation)
{
	Orientation = orientation;
}


