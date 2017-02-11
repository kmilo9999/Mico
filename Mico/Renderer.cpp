#include "Renderer.h"



Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

inline
void Renderer::PrepareRenderer()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline
void Renderer::FinishRenderer()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}
