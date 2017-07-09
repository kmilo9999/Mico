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
#include "ShadowObject.h"


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
	void RenderNormals();
	void ShadowPass();
	void LightModelPass();
	
	void SetGlobalLight(GlobalLight* light);

	
private:
	vec3 RayCast(float mouse_x, float mouse_y);
	Entity* TestRayEntityIntersection(vec3 ray);
	void RenderSelectedVolumen();
	bool RayPlaneIntersection(vec3 plane,  vec3 rayStart, vec3 rayDirection);

	vec2 windowSize;
	Camera* camera;
	Renderer* renderer;
	ShaderProgram lightModelShader;
	ShaderProgram simpleShader;
	ShaderProgram terrainShader;
	ShaderProgram linesShader;
	ShaderProgram shadowShader;
	
	ShadowObject shadowObjet;


	GLFWwindow* window;
	GlobalLight* light;
	glm::mat4 projection;

	Entity* mouseSelectedEntity;
	Terrain* terrain;
	bool hasSelectedEntity;
	vec2 lastMousePosition;
	bool showNormals;
	
};

