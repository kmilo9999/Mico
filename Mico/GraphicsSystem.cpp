#include "GraphicsSystem.h"
#include <iostream>
#include <cassert>
#include <GLFW\glfw3.h>
#include "InputHandler.h"
#include "glm\glm.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Math.h"
#include "WindowSystem.h"


GraphicsSystem::GraphicsSystem( vec2 windowSize):selected(nullptr)
{
	this->windowSize = windowSize;
	
}


GraphicsSystem::~GraphicsSystem()
{
	if (light)
	{
		delete light;
	}
	if (terrain)
	{
		delete terrain;
	}
}

void GraphicsSystem::Init()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		getchar();
		assert(false && "Failed to Initlialize Opengl");
	}
	
	window = WINDOWSYSTEM->GetWindow();
	camera = new Camera(vec3(0,12,8),vec3(0,0,-1));
	InputHandler::GetInstance()->addObserver(camera);
	InputHandler::GetInstance()->addObserver(this);

	modelShader.LoadShaders("Simple.vertexshader", "Simple.fragmentshader");
	lightModelShader.LoadShaders("LightModel.vertexshader", "LightModel.fragmentshader");
	terrainShader.LoadShaders("Terrain.vertexshader","Terrain.fragmentshader");

	modelShader.addUniform("ProjectionMatrix");
	modelShader.addUniform("ViewMatrix");
	modelShader.addUniform("ModelMatrix");
	modelShader.addUniform("light.position");
	modelShader.addUniform("light.ambient");
	modelShader.addUniform("light.diffuse");
	modelShader.addUniform("light.specular");
	modelShader.addUniform("material.ambient");
	modelShader.addUniform("material.diffuse");
	modelShader.addUniform("material.specular");
	modelShader.addUniform("material.shininess");
	modelShader.addUniform("ViewPos");

	lightModelShader.addUniform("ProjectionMatrix");
	lightModelShader.addUniform("ViewMatrix");
	lightModelShader.addUniform("ModelMatrix");
	lightModelShader.addUniform("LightColor");

	terrainShader.addUniform("ProjectionMatrix");
	terrainShader.addUniform("ViewMatrix");
	terrainShader.addUniform("ModelMatrix");



}

void GraphicsSystem::Update()
{
	camera->Update();
	RenderEntities();
	RenderGlobalLight();
	RenderTerrain();
	RenderSelectedVolumen();
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
		case Event::MouseButtonPressed:
			switch (evt.mouseButton.button)
			{
			 case GLFW_MOUSE_BUTTON_LEFT:
				 vec3 ray = RayCast(evt.mouseButton.x, evt.mouseButton.y);
				 TestRayEntityIntersection(ray);
				break;
			};
		    break;
		default:
			break;


	}
}

void GraphicsSystem::InitRender()
{
	
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glViewport(0, 0, windowSize.x, windowSize.y);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	projection = glm::perspective(camera->foV, windowSize.x / windowSize.y, 0.1f, 1000.0f);
}

void GraphicsSystem::InitScene()
{

	light = new GlobalLight(vec3(0.0f, 15.0f, 0.0f), vec3(1.0, 1.0, 1.0));


	light->SetModel(EntityManager::GetInstance()->GetModel("cube"));
	light->SetAmbient(vec3(0.2f));
	light->SetDiffuse(vec3(0.5));
	light->SetSpecular(vec3(1.0f));


	SelectableBoundingVolumen* sphere = new BoundingSphere(light->GetPosition(), 0.7f);
	sphere->SetModel(EntityManager::GetInstance()->GetModel("sphere"));

	light->SetBoundingVolumen(sphere);
	light->SetIntensity(0.5f);

	terrain = new Terrain(0.0f, 0.0f);
}

void GraphicsSystem::FinishRender()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void GraphicsSystem::RenderEntities()
{
	//glm::mat4 view = glm::lookAt(vec3(0,0,4),vec3(0,0,0), vec3(0,1,0));
	glm::mat4 view = camera->GetView();
	modelShader.start();
	modelShader.setUniform("ProjectionMatrix", projection);
	modelShader.setUniform("ViewMatrix",view);
	modelShader.setUniform("light.position", light->GetPosition());
	modelShader.setUniform("light.ambient", light->GetAmbient());
	modelShader.setUniform("light.diffuse", light->GetDiffuse());
	modelShader.setUniform("light.specular", light->GetSpecular());

	modelShader.setUniform("ViewPos", camera->position);
	//Render models
	
	map<TexturedModel*, vector<Entity*>>::iterator it = EntityManager::GetInstance()->GetEntities().begin();
	map<TexturedModel*, vector<Entity*>>::iterator end = EntityManager::GetInstance()->GetEntities().end();
	for (; it != end; ++it)
	{
		TexturedModel* model = it->first;
		if (model->NumTextures() > 0)
		{
			model->BindTexture(modelShader);
		}
		vector<Entity*> batch = it->second;
		for (Entity* entity : batch)
		{
			glm::mat4 modelMatrix = Math::CreateTransformationMatrix(entity->GetPosition(), entity->GetOrientation(), entity->GetScale());
			modelShader.setUniform("ModelMatrix", modelMatrix);
			model->Draw();
			
		}
		model->UnBind();

	}
	modelShader.stop();
	
}

void GraphicsSystem::RenderGlobalLight()
{
	lightModelShader.start();
	lightModelShader.setUniform("ProjectionMatrix", projection);
	lightModelShader.setUniform("ViewMatrix", camera->GetView());
	lightModelShader.setUniform("ModelMatrix", Math::CreateTransformationMatrix(light->GetPosition(), quat(), vec3(0.5, 0.5, 0.5)));
	lightModelShader.setUniform("LightColor", vec4(light->GetColor(), 1));
	light->GetModel()->Bind();
	light->GetModel()->Draw();
	light->GetModel()->UnBind();
	lightModelShader.stop();
	

}

void GraphicsSystem::RenderTerrain()
{
	
	terrainShader.start();
	terrainShader.setUniform("ProjectionMatrix", projection);
	terrainShader.setUniform("ViewMatrix", camera->GetView());
	vec3 position(terrain->GetWidthX(), 9.5, terrain->GetWidthZ());
	terrainShader.setUniform("ModelMatrix", Math::CreateTransformationMatrix(position, quat(), vec3(0.5, 0.5, 0.5)));
	terrain->GetModel()->Bind();
	terrain->GetModel()->Draw();
	terrain->GetModel()->UnBind();
	terrainShader.stop();
}

void GraphicsSystem::RenderSelectedVolumen()
{
	if (selected)
	{
			BoundingSphere* volumen = dynamic_cast<BoundingSphere*>(selected);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			lightModelShader.start();
			lightModelShader.setUniform("ProjectionMatrix", projection);
			lightModelShader.setUniform("ViewMatrix", camera->GetView());
			lightModelShader.setUniform("ModelMatrix", Math::CreateTransformationMatrix(volumen->GetCenter(), quat(), vec3(volumen->GetRadius(), volumen->GetRadius(), volumen->GetRadius())));
			lightModelShader.setUniform("LightColor", vec4(0.5, 0.0, 0.5, 0.4));
			volumen->GetModel()->Bind();
			volumen->GetModel()->Draw();
			volumen->GetModel()->UnBind();
			lightModelShader.stop();
			glDisable(GL_BLEND);
	}
	

}

void GraphicsSystem::SetGlobalLight(GlobalLight* light)
{
	this->light = light;
}

vec3 GraphicsSystem::RayCast(float mouse_x, float mouse_y)
{
	vec4 rayClip((2.0f * mouse_x) / windowSize.x - 1.0f,
		1.0f - (2.0f *  mouse_y) / windowSize.y, -1.0, 1.0);
	mat4 projection = perspective(camera->foV, windowSize.x / windowSize.y, 0.1f, 1000.0f);
	vec4 rayEye = inverse(projection) * rayClip;
	rayEye.z = -1.0f; 
	rayEye.w = 0.0f;
	mat4 view = camera->GetView() ;
	mat4 invView = inverse(view);
	vec4 temp = invView*rayEye;
	vec3 rayWorld(temp.x, temp.y, temp.z);
	rayWorld = normalize(rayWorld);
	return rayWorld;
}

void GraphicsSystem::TestRayEntityIntersection(vec3 ray)
{
	EntityManager::EntityMap::iterator it = EntityManager::GetInstance()->GetEntities().begin();
	EntityManager::EntityMap::iterator end = EntityManager::GetInstance()->GetEntities().end();
	for (;it !=end; ++it)
	{
		vector<Entity*> entities = it->second;
		for(Entity* entity: entities)
		{
			if (entity->GetBoundingVolumen()->RayInstersection(camera->position, ray))
			{
				selected = static_cast<SelectableBoundingVolumen*>(entity->GetBoundingVolumen());
				return;
			}
		}
	}

	if (light->GetBoundingVolumen()->RayInstersection(camera->position, ray))
	{
		selected = static_cast<SelectableBoundingVolumen*>(light->GetBoundingVolumen());
		return;
	}

	selected = nullptr;

}

