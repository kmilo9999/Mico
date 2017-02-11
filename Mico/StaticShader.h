#pragma once
#include "ShaderProgram.h"

class StaticShader: public ShaderProgram
{
public:
	StaticShader();
	~StaticShader();
	void LoadModelMatrix(mat4 modelMatrix);

protected:
	virtual void BindAttributes();
	virtual void GetAllUniformLocations();

private:
	int locationModelMatrix;
};

