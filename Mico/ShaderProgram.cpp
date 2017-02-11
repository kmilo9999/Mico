#include "ShaderProgram.h"
#include <string>
#include <fstream>
#include <vector>
#include <cassert>


ShaderProgram::ShaderProgram(const char * vertex_filenname, const char * fragment_filename)
{
	int vertexShaderID = LoadShader(vertex_filenname, GL_VERTEX_SHADER);
	int fragmentShaderID = LoadShader(fragment_filename, GL_FRAGMENT_SHADER);

	// Link the program
	printf("Linking program\n");
	
	progarmId = glCreateProgram();
	glAttachShader(progarmId, vertexShaderID);
	glAttachShader(progarmId, fragmentShaderID);

	BindAttributes();

	glLinkProgram(progarmId);

	VerifiProgram(progarmId);

	glDetachShader(progarmId, vertexShaderID);
	glDetachShader(progarmId, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	GetAllUniformLocations();

}

ShaderProgram::~ShaderProgram()
{
}

inline
void ShaderProgram::start()
{
	glUseProgram(progarmId);
}

inline
void ShaderProgram::stop()
{
	glUseProgram(0);
}

void ShaderProgram::BindAttribute(int attribute, const char * variableName)
{
	glBindAttribLocation(progarmId, attribute, variableName);
}

int ShaderProgram::GetUniformLocation(const char * variableName)
{
	return glGetUniformLocation(progarmId, variableName);
}

void ShaderProgram::LoadFloat(int location, float value)
{
	glUniform1f(location, value);
}

void ShaderProgram::LoadVector(int location, vec3 value)
{
	glUniform3fv(location, 1 , glm::value_ptr(value));
}

void ShaderProgram::LoadBoolean(int location, bool value)
{
	if (value)
		glUniform1i(location, 1);
	else
		glUniform1i(location, 0);
}

void ShaderProgram::LoadMatrix(int location, mat4 value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

GLuint ShaderProgram::LoadShader(const char * filenname, int type)
{
	GLuint shaderId = glCreateShader(type);
	// Read the Vertex Shader code from the file
	std::string ShaderCode;
	std::ifstream ShaderStream(filenname, std::ios::in);

	if (ShaderStream.is_open()) {
		std::string Line = "";
		while (getline(ShaderStream, Line))
			ShaderCode += "\n" + Line;
		ShaderStream.close();
	}
	else {
		printf("Impossible to open %s !\n", filenname);
		getchar();
		return 0;
	}

	// Compile Shader
	printf("Compiling shader : %s\n", filenname);
	char const * SourcePointer = ShaderCode.c_str();
	glShaderSource(shaderId, 1, &SourcePointer, NULL);
	glCompileShader(shaderId);

	VerifiProgram(shaderId);
	return shaderId;
}

inline
void ShaderProgram::VerifiProgram(GLuint programId)
{

	GLint Result = GL_FALSE;
	int InfoLogLength;
	// Check Vertex Shader
	glGetShaderiv(programId, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(programId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(programId, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	assert(Result);
}