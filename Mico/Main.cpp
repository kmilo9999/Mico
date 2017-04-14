#include "../lib/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <FreeImage\FreeImage.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Texture.h"
#include "Mico.h"

#ifdef _WIN32
	#define APIENTRY __stdcall
#endif


// The MAIN function, from here we start the application and run the game loop
int main()
{

	Mico::GetInstance()->Initialize();
	Mico::GetInstance()->Run();

	// Terminates GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

//using namespace std;
//using namespace glm;
//
//vector<vec3> vertices1{  vec3(0.5f,  0.5f, 0.0f),
//						vec3(0.5f, -0.5f, 0.0f),
//						vec3(-0.5f, -0.5f, 0.0f),
//						vec3(-0.5f,  0.5f, 0.0f) };
//
//vector<int> indexes1{ 0,1,3,
//					  1,2,3 };
//
//vector<vec2> textcoor{
//	vec2(1.0f, 1.0f), // Top Right
//	vec2(1.0f, 0.0f), // Bottom Right
//	vec2(0.0f, 0.0f), // Bottom Left
//	vec2(0.0f, 1.0f)  // Top Left 
//};
//
////vector<vec3> vertices1{ vec3(-0.5f,  0.5f, 0.0f),
////						vec3(-0.5f, -0.5f, 0.0f),
////						vec3(0.5f, -0.5f, 0.0f),
////						vec3(0.5f,  0.5f, 0.0f) };
////
////vector<int> indexes1{ 0,1,3,
////					  3,1,2 };
//
//GLuint LoadShader(const char * filenname, int type)
//{
//	GLuint shaderId = glCreateShader(type);
//	// Read the Vertex Shader code from the file
//	std::string ShaderCode;
//	std::ifstream ShaderStream(filenname, std::ios::in);
//
//	if (ShaderStream.is_open()) {
//		std::string Line = "";
//		while (getline(ShaderStream, Line))
//			ShaderCode += "\n" + Line;
//		ShaderStream.close();
//	}
//	else {
//		printf("Impossible to open %s !\n", filenname);
//		getchar();
//		return 0;
//	}
//
//	// Compile Shader
//	printf("Compiling shader : %s\n", filenname);
//	char const * SourcePointer = ShaderCode.c_str();
//	glShaderSource(shaderId, 1, &SourcePointer, NULL);
//	glCompileShader(shaderId);
//
//	GLint Result = GL_FALSE;
//	int InfoLogLength;
//	// Check Vertex Shader
//	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &Result);
//
//	if (Result == FALSE) {
//		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
//		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
//		glGetShaderInfoLog(shaderId, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
//		printf("%s\n", &VertexShaderErrorMessage[0]);
//	}
//	return shaderId;
//}
//
//int LoadShaders(const char * vertex_filenname, const char * fragment_filename)
//{
//	int vertexShaderID = LoadShader(vertex_filenname, GL_VERTEX_SHADER);
//	int fragmentShaderID = LoadShader(fragment_filename, GL_FRAGMENT_SHADER);
//	int IsLinked;
//	// Link the program
//	printf("Linking program\n");
//
//	int progarmId = glCreateProgram();
//	glAttachShader(progarmId, vertexShaderID);
//	glAttachShader(progarmId, fragmentShaderID);
//
//	glBindAttribLocation(progarmId, 0, "vertexPosition");
//	
//
//	glLinkProgram(progarmId);
//
//	glGetProgramiv(progarmId, GL_LINK_STATUS, (int *)&IsLinked);
//	if (IsLinked == FALSE)
//	{
//		int maxLength;
//		glGetProgramiv(progarmId, GL_INFO_LOG_LENGTH, &maxLength);
//		std::vector<char> VertexShaderErrorMessage(maxLength + 1);
//		glGetProgramInfoLog(progarmId, maxLength, &maxLength, &VertexShaderErrorMessage[0]);
//		printf("%s\n", &VertexShaderErrorMessage[0]);
//		return -1;
//	}
//
//	glDetachShader(progarmId, vertexShaderID);
//	glDetachShader(progarmId, fragmentShaderID);
//
//	glDeleteShader(vertexShaderID);
//	glDeleteShader(fragmentShaderID);
//	return progarmId;
//
//}
//
//
//int main()
//{
//	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
//	// Init GLFW
//	glfwInit();
//	// Set all the required options for GLFW
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//
//	// Create a GLFWwindow object that we can use for GLFW's functions
//	GLFWwindow* window = glfwCreateWindow(800, 600, "MICO", NULL, NULL);
//	glfwMakeContextCurrent(window);
//	if (window == NULL)
//	{
//		glfwTerminate();
//		getchar();
//		assert(false && "Failed to create GLFW window");
//	}
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//	{
//		glfwTerminate();
//		getchar();
//		assert(false && "Failed to Initlialize Opengl");
//	}
//		
//	GLuint vertex_buffer, texturecoor_buffer, vertex_shader, fragment_shader, program, vao;
//	GLint mvp_location, vpos_location, vcol_location;
//
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//
//	glGenBuffers(1, &vertex_buffer);
//	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
//	glBufferData(GL_ARRAY_BUFFER, vertices1.size() * sizeof(vec3), &vertices1[0], GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid*)0);
//	glEnableVertexAttribArray(0);
//
//	glGenBuffers(1, &texturecoor_buffer);
//	glBindBuffer(GL_ARRAY_BUFFER, texturecoor_buffer);
//	glBufferData(GL_ARRAY_BUFFER, textcoor.size() * sizeof(vec2), &textcoor[0], GL_STATIC_DRAW);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (GLvoid*)0);
//	glEnableVertexAttribArray(1);
//
//	GLuint ebo;
//	glGenBuffers(1, &ebo);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes1.size() * sizeof(int),
//		&indexes1[0], GL_STATIC_DRAW);
//
//	glBindVertexArray(0);
//
//	Texture* texture = new Texture(GL_TEXTURE_2D, "../Resources/Textures/dragonballzsuper.jpg");
//
//	program = LoadShaders("Simple.vertexshader", "Simple.fragmentshader");
//	mvp_location = glGetUniformLocation(program, "MVP");
//	
//	int width, height;
//	mat4x4 m, p,v, mvp;
//	glfwGetFramebufferSize(window, &width, &height);
//
//
//	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
//	glm::mat4 View = glm::lookAt(glm::vec3(0, 0, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0) );
//
//	glViewport(0, 0, width, height);
//
//	while (!glfwWindowShouldClose(window))
//	{
//		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
//		glClear(GL_COLOR_BUFFER_BIT );
//
//		glUseProgram(program);
//		
//		glm::mat4 ModelMatrix = glm::mat4(1.0);
//		mvp = Projection * View * ModelMatrix;
//
//
//		glUseProgram(program);
//		glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);
//		// Bind Textures using texture units
//		
//		texture->Bind(0);
//		glUniform1i(glGetUniformLocation(program, "texture"), 0);
//
//		glBindVertexArray(vao);
//		glDrawElements(GL_TRIANGLES, indexes1.size(), GL_UNSIGNED_INT, 0);
//		glBindVertexArray(0);
//
//		glDisableVertexAttribArray(0);
//
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//	delete texture;
//	glfwDestroyWindow(window);
//	glfwTerminate();
//	exit(EXIT_SUCCESS);
//}
//
//
//
