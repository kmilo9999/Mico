#pragma once
#include "Component.h"
#include "TexturedModel.h"
#include "ShaderProgram.h"
#include "TransformationComponent.h"
#include "Material.h"
#include "FBO.h"

class GraphicsComponent : public Component
{
public:
	GraphicsComponent(TexturedModel* model, Material* material);
	~GraphicsComponent();
	virtual void Update();
	virtual void Initialize(Entity*);

	virtual void ShutDown();

	void Draw(ShaderProgram& shader);

	TexturedModel* GetTexturedModel();
	void  SetTexturedModel(TexturedModel* model);

	void SetMaterial(Material* material);
	Material* GetMaterial();


	mat4& GetModelMatrix();
	void SetModelMatrix(mat4& modelMatrix);

	Fbo* getShadowsFbo();
	void setShadowsFbo(Fbo* fbo);

	void bindModelTextures();

private:
	TexturedModel* myModel;
	Material*  myMaterial;
	Fbo* myShadowFBO;

	// a pointer to its owner transformation component
	TransformationComponent* transformation;
	mat4 myModelMatrix;
};

