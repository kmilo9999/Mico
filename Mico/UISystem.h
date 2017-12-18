#pragma once
#include <glad\glad.h>
#include "System.h"
#include <GLFW\glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW\glfw3native.h>
#include "imgui.h"
#include "Observable.h"



class UISystem:public System, public Observable
{
public:
	UISystem();
	~UISystem();
	virtual void Init();
	virtual void Update(float dt);
	virtual std::string className();
	void ImGuiNewFrame();
	void ImGuiShutdown();
private:
	bool showNormals = false;
	bool showModel = true;
};
/// Global pointer to the Graphics System.
extern UISystem * UISYSTEM;
