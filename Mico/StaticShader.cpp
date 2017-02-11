#include "StaticShader.h"


#define VERTEX_FILE "Simple.vertexshader"
#define FRAGMENT_FILE "Simple.fragmentshader"

StaticShader::StaticShader():ShaderProgram(VERTEX_FILE, FRAGMENT_FILE)
{
}


StaticShader::~StaticShader()
{
}

void StaticShader::LoadModelMatrix(mat4 modelMatrix)
{
	LoadMatrix(locationModelMatrix, modelMatrix);
}

void StaticShader::BindAttributes()
{
	BindAttribute(0, "vertexPosition");
	BindAttribute(1, "vertexColor");
	BindAttribute(2, "textCoord");

}

void StaticShader::GetAllUniformLocations()
{
	locationModelMatrix = GetUniformLocation("ModelMatrix");
}
