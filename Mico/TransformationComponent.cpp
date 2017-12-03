#include "TransformationComponent.h"
#include "Math.h"


TransformationComponent::TransformationComponent(vec3 position, 
	quat orientation, vec3 scale)
	: myPosition(position)
	, myOrientation(orientation)
	, myScale(scale)
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
	return myPosition;
}

void TransformationComponent::SetPosition(vec3 position)
{
	myPosition = position;
	/*if (boundingVolumen)
	{
		boundingVolumen->SetPotision(Position);
	}*/
}

vec3 TransformationComponent::GetScale()
{
	return myScale;
}

void TransformationComponent::SetScale(vec3 scale)
{
	myScale = scale;
}

quat TransformationComponent::GetOrientation()
{
	return myOrientation;
}

void TransformationComponent::SetOrientation(quat orientation)
{
	myOrientation = orientation;
}

void TransformationComponent::Rotate(quat rotation)
{
	myOrientation = myOrientation * rotation;
}


