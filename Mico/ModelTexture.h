#pragma once
#include <glad\glad.h>

class ModelTexture
{
public:
	ModelTexture(int id);
	~ModelTexture();

	GLuint GetId();
	void SetId(GLuint nid);

private:
	GLuint id;

};

