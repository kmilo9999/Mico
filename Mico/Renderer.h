#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "TexturedModel.h"
#include "Entity.h"
#include <map>
#include <vector>

using namespace std;

class Renderer
{
public:
	Renderer(GLFWwindow*);
	~Renderer();
	void  PrepareRenderer();
	void  FinishRenderer();
private:
	map<TexturedModel*, vector<Entity*>> entities;
	GLFWwindow* window;
};

