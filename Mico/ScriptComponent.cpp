#include "ScriptComponent.h"



ScriptComponent::ScriptComponent()
{
}


ScriptComponent::~ScriptComponent()
{
}

void ScriptComponent::Update()
{

}

void ScriptComponent::Initialize(Entity * owner)
{
	myOwner = owner;
}

void ScriptComponent::ShutDown()
{
}
