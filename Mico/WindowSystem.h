#pragma once
#include "System.h"
#include <GLFW\glfw3.h>
#include <string>
#include "InputHandler.h"
#include "WindowHandler.h"
#include <glm\glm.hpp>

using namespace glm;

class WindowSystem:public System
{
public:
	WindowSystem(int width, int height);
	~WindowSystem();
	virtual void Init();
	virtual void Update(float dt);
	virtual std::string className();
	GLFWwindow* GetWindow();
	GLuint GetWidth();
	GLuint GetHeight();
	vec2 Getsize();

private:
	
	static void Keycallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mode);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void WindowsSizeCallback(GLFWwindow* window, int width, int height);

	GLFWwindow* window;
	/*InputHandler* inputHandler;*/
	// Window dimensions
	GLuint width, height;
};


/// Global pointer to the Graphics System.
extern WindowSystem * WINDOWSYSTEM;

