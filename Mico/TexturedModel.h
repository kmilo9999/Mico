#pragma once
#include "RawModel.h"
#include "ModelTexture.h"

class TexturedModel
{
public:
	TexturedModel(RawModel* model, ModelTexture texture);
	~TexturedModel();

	RawModel* GetModel();
	void SetModel(RawModel* nModel);

	ModelTexture GetModelTexture();
	void SetModelTexture(ModelTexture texture);

	void Bind();
	void Draw();
	void UnBind();

protected:
	RawModel* model;
	ModelTexture texture;
};

