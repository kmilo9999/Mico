#pragma once
#include "RawModel.h"
#include "Texture.h"
#include "vector"
#include "glm\glm.hpp"
#include "ShaderProgram.h"

using namespace std;
using namespace glm;

class TexturedModel
{
public:
	TexturedModel(RawModel* model, vector<Texture*>&  textures);
	~TexturedModel();

	RawModel* GetModel();
	void SetModel(RawModel* nModel);

	void BindTexture(ShaderProgram& shader);
	void Bind();
	void Draw();
	void UnBind();

	void Addtexture(Texture* texture);

	void SetId(string id);

	string GetId();
	
	unsigned int NumTextures();

	vec4 DiffuseColor;
	vec4 SpecularColor;
	vec4 AmbientColor;
	float Shininess;

protected:
	RawModel* model;

	vector<Texture*>  textures;
	string id;

};

struct	TexturedModelCompare
{
	bool operator() (TexturedModel* lhs, TexturedModel* rhs) const
	{
		return lhs->GetModel()->getVao() < rhs->GetModel()->getVao();
	}
};