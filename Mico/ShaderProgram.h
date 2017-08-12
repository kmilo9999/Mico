#pragma once
#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>
#include <string>

using namespace glm;
using namespace std;

class ShaderProgram
{
public:
	ShaderProgram();
	void LoadShaders(const char* vertex_filenname, const char* fragment_filename, const char* geometry_file = NULL);
	~ShaderProgram();
	void start();
	void stop();

	void addUniform(const char* name);

	void setUniformi(const char* name, int value);
	void setUniformf(const char* name, float value);
	void setUniform(const char* name, vec3 vector);
	void setUniform(const char* name, vec4 vector);
	void setUniform(const char* name, mat4 vector);
	
	GLuint GetProgramId();

private:

	GLuint progarmId;
	void VerifiProgram(GLuint programId);
	bool InUse();
	GLuint LoadShader(const char* vertex_filenname, int type);
	unordered_map<string, GLint> uniforms;
};

