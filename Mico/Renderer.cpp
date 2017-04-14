#include "Renderer.h"



Renderer::Renderer(GLFWwindow* window):window(window)
{
}


Renderer::~Renderer()
{
}


void Renderer::PrepareRenderer()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Renderer::FinishRenderer()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}
