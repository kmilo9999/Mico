#include "GraphicsSystem.h"
#include <iostream>
#include <cassert>
#include <GLFW\glfw3.h>
#include "InputHandler.h"
#include "glm\glm.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

GraphicsSystem::GraphicsSystem(vec2 windowSize)
{
	this->windowSize = windowSize;
}


GraphicsSystem::~GraphicsSystem()
{
}

void GraphicsSystem::Init()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		getchar();
		assert(false && "Failed to Initlialize Opengl");
	}
	
	camera = new Camera();
	InputHandler::GetInstance()->addObserver(camera);

	renderer = new Renderer();
}

void GraphicsSystem::Update()
{
	
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)windowSize.x / (float)windowSize.y, 0.1f, 100.0f);
	glm::mat4 view = camera->GetView();

	renderer->PrepareRenderer();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glViewport(0, 0, windowSize.x, windowSize.y);


	renderer->FinishRenderer();
}

std::string GraphicsSystem::className()
{
	return "MICO::GraphicsSystem";
}

void GraphicsSystem::onNotify(Event & evt)
{
	switch (evt.type)
	{
		case Event::Resized:
			windowSize.x = evt.size.width;
			windowSize.y = evt.size.height;
			break;
		default:
			break;
	}
}

