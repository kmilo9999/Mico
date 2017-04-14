#pragma once
#include "System.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "Renderer.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "GlobalLight.h"
#include "Terrain.h"
#include "EntityManager.h"

class GraphicsSystem:public System, public Observer
{
public:

	GraphicsSystem( vec2 windowSize);
	~GraphicsSystem();

	virtual void Init();
	virtual void Update();
	virtual std::string className();
	virtual void onNotify(Event& event);

	void InitRender();
	void InitScene();
	void FinishRender();
	void RenderEntities();
	void RenderGlobalLight();
	void RenderTerrain();
	
	void SetGlobalLight(GlobalLight* light);

	
private:
	vec3 RayCast(float mouse_x, float mouse_y);
	void TestRayEntityIntersection(vec3 ray);
	void RenderSelectedVolumen();
	vec2 windowSize;
	Camera* camera;
	Renderer* renderer;
	ShaderProgram modelShader;
	ShaderProgram lightModelShader;
	ShaderProgram terrainShader;
	GLFWwindow* window;
	GlobalLight* light;
	glm::mat4 projection;

	SelectableBoundingVolumen* selected;
	Terrain* terrain;
};

