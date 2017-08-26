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
	TexturedModel(GLenum mode,RawModel* model);
	TexturedModel(const TexturedModel& other);
	TexturedModel& operator=(const TexturedModel& other);

	~TexturedModel();

	RawModel* GetModel();
	RawModel* GetModel() const;

	void SetModel(RawModel* nModel);

	void BindTexture(std::vector<Texture*>& textures);

	//used for shadows (for now)
	void BindTexture(int textureUnit, int textureId);
	

	void Draw();
	void DrawNormals();
	void UnBind();


	void SetId(string id);

	string GetId();
	
	void SetDrawMode(GLenum mode);
	GLenum GetDrawModel();

protected:
	RawModel* myModel;
	GLenum myDrawMode;
	string id;

};

struct	TexturedModelCompare
{
	bool operator() (TexturedModel* lhs, TexturedModel* rhs) const
	{
		return lhs->GetModel()->getVao() < rhs->GetModel()->getVao();
	}
};