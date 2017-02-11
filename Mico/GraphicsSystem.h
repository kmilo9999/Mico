#pragma once
#include "System.h"
#include <glad\glad.h>
#include "Renderer.h"
#include "Camera.h"

class GraphicsSystem:public System, public Observer
{
public:

	GraphicsSystem(vec2 windowSize);
	~GraphicsSystem();

	virtual void Init();
	virtual void Update();
	virtual std::string className();
	virtual void onNotify(Event& event);

private:
	vec2 windowSize;
	Camera* camera;
	Renderer* renderer;
};

