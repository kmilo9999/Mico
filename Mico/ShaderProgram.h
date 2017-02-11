#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class ShaderProgram
{
public:
	ShaderProgram(const char* vertex_filenname, const char* fragment_filename);
	~ShaderProgram();
	void start();
	void stop();

protected:
	virtual void BindAttributes() = 0;
	virtual void GetAllUniformLocations() = 0;

	void BindAttribute(int attribute, const char* variableName);
	int GetUniformLocation(const char* variableName);
	void LoadFloat(int location, float value);
	void LoadVector(int location, vec3 value);
	void LoadBoolean(int location, bool value);
	void LoadMatrix(int location, mat4 value);

private:

	GLuint progarmId;
	void VerifiProgram(GLuint programId);
	GLuint LoadShader(const char* vertex_filenname, int type);

};

