#include "GraphicsComponent.h"
#include "TransformationComponent.h"
#include "Entity.h"
#include "Math.h"
#include <cassert>	

GraphicsComponent::GraphicsComponent(TexturedModel* model, Material* material)
	: myModel(model)
	, myMaterial(material)
	, myShadowFBO(0)
{
	ComponentName = "GraphicsComponent";
}


GraphicsComponent::~GraphicsComponent()
{
	if (myModel)
	{
		delete myModel;
	}
	if(myMaterial)
	{ 
		delete myMaterial;
	}
}

void GraphicsComponent::Update()
{
	myModelMatrix = Math::CreateTransformationMatrix(
		transformation->GetPosition(),
		transformation->GetOrientation(), 
		transformation->GetScale());
}

void GraphicsComponent::Initialize(Entity* owner)
{
	myOwner = owner;
	transformation = dynamic_cast<TransformationComponent*>(owner->GetComponent("TransformationComponent"));
	
}

void GraphicsComponent::Draw(ShaderProgram& shader)
{
	shader.setUniform("ModelMatrix", myModelMatrix);

	myModel->Draw();
	myModel->UnBind();
}

void GraphicsComponent::ShutDown()
{
	
}

TexturedModel * GraphicsComponent::GetTexturedModel()
{
	return myModel;
}

void GraphicsComponent::SetTexturedModel(TexturedModel * model)
{
	myModel = model;
}

void GraphicsComponent::SetMaterial(Material * material)
{
	myMaterial = material;
}

Material * GraphicsComponent::GetMaterial()
{
	return myMaterial;
}

mat4& GraphicsComponent::GetModelMatrix()
{
	return myModelMatrix;
}

void GraphicsComponent::SetModelMatrix(mat4 & modelMatrix)
{
	myModelMatrix = modelMatrix;
}

Fbo * GraphicsComponent::getShadowsFbo()
{
	return myShadowFBO;
}

void GraphicsComponent::setShadowsFbo(Fbo*  fbo)
{
	myShadowFBO = fbo;
}

void GraphicsComponent::bindModelTextures()
{
	myModel->BindTexture(myMaterial->GetTextures());

	if (myShadowFBO)
	{
		myModel->BindTexture(1, myShadowFBO->texture);
	}
}

