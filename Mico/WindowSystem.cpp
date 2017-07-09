#include "WindowSystem.h"
#include <iostream>
#include <cassert>
#include "Event.h"

WindowSystem * WINDOWSYSTEM = nullptr;

WindowSystem::WindowSystem(int width, int height):width(width), height(height)
{
	
}


WindowSystem::~WindowSystem()
{
}

void WindowSystem::Init()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "MICO", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		glfwTerminate();
		getchar();
		assert(false && "Failed to create GLFW window");
	}
	
	// Set the required callback functions
	glfwSetKeyCallback(window, WindowSystem::Keycallback);
	glfwSetCursorPosCallback(window, WindowSystem::MouseMoveCallback);
	glfwSetScrollCallback(window, WindowSystem::ScrollCallback);
	glfwSetMouseButtonCallback(window, WindowSystem::MouseButtonCallback);
	glfwSetWindowSizeCallback(window, WindowSystem::WindowsSizeCallback);
	WINDOWSYSTEM = this;
}

void WindowSystem::Update()
{
}

std::string WindowSystem::className()
{
	return "MICO::WindowSystem";
}

GLFWwindow* WindowSystem::GetWindow()
{
	return window;
}

GLuint WindowSystem::GetWidth()
{
	return this->width;
}

GLuint WindowSystem::GetHeight()
{
	return this->height;
}

vec2 WindowSystem::Getsize()
{
	return vec2(width, height);
}

void WindowSystem::Keycallback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
	{
		Event evt(Event::KeyPressed);
		evt.key.code = key;
		InputHandler::GetInstance()->notify(evt);
	}
	else if (action == GLFW_RELEASE)
	{
		Event evt(Event::KeyReleased);
		evt.key.code = key;
		InputHandler::GetInstance()->notify(evt);
	}
}

void WindowSystem::MouseMoveCallback(GLFWwindow * window, double xpos, double ypos)
{

	Event evt(Event::MouseMoved);
	evt.mouseMove.x = (float)xpos;
	evt.mouseMove.y = (float)ypos;
	InputHandler::GetInstance()->notify(evt);
}

void WindowSystem::MouseButtonCallback(GLFWwindow * window, int button, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		Event evt(Event::MouseButtonPressed);
		evt.mouseButton.button = button;
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		evt.mouseButton.x = (float)xpos;
		evt.mouseButton.y = (float)ypos;
		InputHandler::GetInstance()->notify(evt);
	}
	else if (action == GLFW_RELEASE)
	{
		Event evt(Event::MouseButtonReleased);
		evt.mouseButton.button = button;
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		evt.mouseButton.x = (float)xpos;
		evt.mouseButton.y = (float)ypos;
		InputHandler::GetInstance()->notify(evt);
	}
}

void WindowSystem::ScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
	Event evt(Event::MouseWheel);
	evt.mouseWheel.z = static_cast<float>(yoffset);
	InputHandler::GetInstance()->notify(evt);
}

void WindowSystem::WindowsSizeCallback(GLFWwindow * window, int width, int height)
{
	Event evt(Event::Resized);
	evt.size.width = width;
	evt.size.height = height;
	WindowHandler::GetInstance()->notify(evt);
}

