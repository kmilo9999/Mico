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
#include "TransformationComponent.h"
#include "GraphicsComponent.h"
#include "FBO.h"

GraphicsSystem::GraphicsSystem(vec2 windowSize) 
	: mouseSelectedEntity(nullptr)
	, hasSelectedEntity(false)
	, lastMousePosition(0.0f, 0.0f)
	, showNormals(false)
	, ambientColor(0.5)
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

	lightModelShader.LoadShaders("LightModel.vertexshader", "LightModel.fragmentshader");
	simpleShader.LoadShaders("Simple.vertexshader", "Simple.fragmentshader");
	terrainShader.LoadShaders("Terrain.vertexshader","Terrain.fragmentshader");
	linesShader.LoadShaders("NormalLines.vertexshader", "NormalLines.fragmentshader");
	shadowShader.LoadShaders("Shadows.vertexshader","Shadows.fragmentshader");

	lightModelShader.addUniform("ProjectionMatrix");
	lightModelShader.addUniform("ViewMatrix");
	lightModelShader.addUniform("ModelMatrix");
	lightModelShader.addUniform("light.position");
	lightModelShader.addUniform("light.ambient");
	lightModelShader.addUniform("light.diffuse");
	lightModelShader.addUniform("light.specular");
	lightModelShader.addUniform("material.ambient");
	lightModelShader.addUniform("material.diffuse");
	lightModelShader.addUniform("material.specular");
	lightModelShader.addUniform("material.shininess");
	lightModelShader.addUniform("ViewPos");
	lightModelShader.addUniform("hasTexture");
	lightModelShader.addUniform("shadowMap");
	lightModelShader.addUniform("ShadowMatrix");
	lightModelShader.addUniform("isGround");

	
	//lightModelShader.addUniform("shadowMap");

	simpleShader.addUniform("ProjectionMatrix");
	simpleShader.addUniform("ViewMatrix");
	simpleShader.addUniform("ModelMatrix");
	simpleShader.addUniform("LightColor");

	terrainShader.addUniform("ProjectionMatrix");
	terrainShader.addUniform("ViewMatrix");
	terrainShader.addUniform("ModelMatrix");
	terrainShader.addUniform("shadowMap");
	terrainShader.addUniform("ShadowMatrix");

	linesShader.addUniform("ProjectionMatrix");
	linesShader.addUniform("ViewMatrix");
	linesShader.addUniform("ModelMatrix");

	Fbo* shadowframeBuffer = new Fbo();
	shadowframeBuffer->CreateFBO(1024, 1024);

	shadowObjet.setShadowFbo(shadowframeBuffer);

	shadowShader.addUniform("ProjectionMatrix");
	shadowShader.addUniform("ViewMatrix");
	shadowShader.addUniform("ModelMatrix");

	renderToQuadShader.LoadShaders("quad.vertexshader", "quad.fragmentshader");
	renderToQuadShader.addUniform("screenTexture");


	/* Render to quad init*/

	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

}

void GraphicsSystem::Update()
{

	camera->Update();
	projection = glm::perspective(camera->foV, windowSize.x / windowSize.y, 0.1f, 1000.0f);
	light->Update();
	terrain->Update();
	
	ShadowPass();
	
	

	InitRender();
	//RenderTextureToQuad(*shadowObjet.shadowFbo());

	LightModelPass();

	//if (showNormals)
	//{
	//	RenderNormals();
	//}
	RenderGlobalLight();
	/*
	RenderSelectedVolumen();*/
	
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
				 //cout << ray.x << "," << ray.y << "," << ray.z << endl;
				 mouseSelectedEntity = TestRayEntityIntersection(ray);
				 if(mouseSelectedEntity)
				 { 
					 hasSelectedEntity = true;
				 }
				 else
				 {
					 hasSelectedEntity = false;
				 }
				// TestRayFarPlanesIntersection(ray);
				break;
			};
		    break;
		case Event::MouseMoved:
			{
				if (!hasSelectedEntity)
				{
					lastMousePosition.x = evt.mouseMove.x;
					lastMousePosition.y = evt.mouseMove.y;
					break;
				}
				if (hasSelectedEntity)
				{
					vec3 rightVector = normalize(cross(camera->front, camera->up))*0.06f;
					vec3 upvector = normalize(camera->up)*0.06f;
					float deltaX = (evt.mouseMove.x - lastMousePosition.x);
					float deltaY = (evt.mouseMove.y - lastMousePosition.y);
					TransformationComponent* transform = dynamic_cast<TransformationComponent*>(mouseSelectedEntity->GetComponent("TransformationComponent"));
					vec3 newPosition = transform->GetPosition();
					float absolutX = abs(deltaX);
					float absolutY = abs(deltaY);

					if (deltaX && deltaX < 0 &&   absolutX > 2.5)
					{
						newPosition -=  rightVector;
					}
					else if(deltaX && deltaX > 0 && absolutX > 2.5)
					{
						newPosition +=   rightVector;
					}

					if (deltaY && deltaY < 0 && absolutY > 2.5)
					{
						newPosition +=  upvector;
					}
					else if (deltaY && deltaY > 0 && absolutY > 2.5)
					{
						newPosition -= upvector;
					}
					transform->SetPosition(newPosition);
					lastMousePosition.x = evt.mouseMove.x;
					lastMousePosition.y = evt.mouseMove.y;
				}
			}
			break;
		case Event::MouseButtonReleased:
			switch (evt.mouseButton.button)
			{
			case GLFW_MOUSE_BUTTON_LEFT:
				hasSelectedEntity = false;
				break;
			};
			break;
		case Event::UI:
			switch (evt.ui.type)
			{
			  case Event::ShowNormals:
				  showNormals = evt.ui.value.boolValue;
				  break;
			  default:
				  break;
			}
			break;
		default:
			break;


	}
}

void GraphicsSystem::InitRender()
{
	
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//glCullFace(GL_BACK);
	glViewport(0, 0, windowSize.x, windowSize.y);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
}

void GraphicsSystem::InitScene()
{
	light = new GlobalLight();

	light->SetAmbient(vec3(0.5f));
	light->SetDiffuse(vec3(0.5));
	light->SetSpecular(vec3(1.0f));

	TransformationComponent* lightTransformation = 
		new TransformationComponent(vec3(1.0f,28.0f,0.3f),quat(), vec3(0.3f, 0.3f, 0.3f));
	light->AddComponent(lightTransformation);

	Texture* targetTtexture = new Texture(GL_TEXTURE_2D, "../Resources/Textures/texture_sample.jpg");
	Material* material = new Material(vec3(1.0f, 0.5f, 0.31f), vec3(1.0f, 0.5f, 0.31f), vec3(0.2f, 0.2f, 0.2f), 64.0f);
	material->AddTexture(targetTtexture);
	GraphicsComponent* graphics = new GraphicsComponent(EntityManager::GetInstance()->GetModel("cube"), material);
	light->AddComponent(graphics);

	SelectableBoundingVolumen* boundingVolumen = new BoundingSphere( 0.7f);
	boundingVolumen->SetModel(EntityManager::GetInstance()->GetModel("sphere"));
	light->AddComponent(boundingVolumen);

	light->SetIntensity(0.5f);


	light->Initialize();

	terrain = new Terrain(0.0f, 0.0f);
	

}

void GraphicsSystem::FinishRender()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void GraphicsSystem::RenderEntities()
{
	
	TransformationComponent* lightTransform = dynamic_cast<TransformationComponent*>(light->GetComponent("TransformationComponent"));
	if (lightTransform)
	{
		
		lightModelShader.start();
		lightModelShader.setUniform("ProjectionMatrix", projection);
		lightModelShader.setUniform("ViewMatrix", camera->GetView());
		lightModelShader.setUniform("light.position", lightTransform->GetPosition());
		lightModelShader.setUniform("light.ambient", light->GetAmbient());
		lightModelShader.setUniform("light.diffuse", light->GetDiffuse());
		lightModelShader.setUniform("light.specular", light->GetSpecular());
		lightModelShader.setUniform("ViewPos", camera->position);
		lightModelShader.setUniformi("isGround", 0);

		//add shadow
		mat4x4 trans = translate(mat4(), vec3(0.5f, 0.5f, 0.5f));
		mat4x4 sca = scale(mat4(), vec3(0.5f, 0.5f, 0.5f));
		mat4x4 B = trans*sca;

		mat4& viewMatrixLightPOV = glm::lookAt(lightTransform->GetPosition(), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		mat4x4 shadowMatrix = B*shadowObjet.shadowProjectionMatrix() * viewMatrixLightPOV;

		lightModelShader.setUniform("ShadowMatrix", shadowMatrix);
		lightModelShader.setUniformi("shadowMap", 1);

		//Render models
		vector<Entity*>::iterator it = EntityManager::GetInstance()->GetEntities().begin();
		vector<Entity*>::iterator end = EntityManager::GetInstance()->GetEntities().end();
		for (; it != end; ++it)
		{

			GraphicsComponent* graphicsComponent = dynamic_cast<GraphicsComponent*>((*it)->GetComponent("GraphicsComponent"));
			if (graphicsComponent)
			{
				Material* material = graphicsComponent->GetMaterial();

				if (material->GetTextures().size() > 0)
				{
					lightModelShader.setUniformi("hasTexture", 1);
				}
				else
				{ 
					lightModelShader.setUniformi("hasTexture", 0);
				}

				lightModelShader.setUniform("material.ambient", ambientColor);
				lightModelShader.setUniform("material.diffuse", material->GetDiffuse());
				lightModelShader.setUniform("material.specular", material->GetSpecular());
				lightModelShader.setUniformf("material.shininess", material->GetShinines());
				graphicsComponent->setShadowsFbo(shadowObjet.shadowFbo());
				graphicsComponent->bindModelTextures();
				graphicsComponent->Draw(lightModelShader);
			}


		}
		lightModelShader.stop();
	}
	
}

void GraphicsSystem::RenderGlobalLight()
{

	simpleShader.start();
	simpleShader.setUniform("ProjectionMatrix", projection);
	simpleShader.setUniform("ViewMatrix", camera->GetView());
	simpleShader.setUniform("LightColor", vec4(light->GetColor(), 1));

	GraphicsComponent* lightGraphics = dynamic_cast<GraphicsComponent*>(light->GetComponent("GraphicsComponent"));


	lightGraphics->Draw(simpleShader);
	simpleShader.stop();
	

}

void GraphicsSystem::RenderTerrain()
{
	TransformationComponent* lightTransform = dynamic_cast<TransformationComponent*>(light->GetComponent("TransformationComponent"));
	
	if (lightTransform)
	{
		lightModelShader.start();
		lightModelShader.setUniform("ProjectionMatrix", projection);
		lightModelShader.setUniform("ViewMatrix", camera->GetView());
		lightModelShader.setUniform("light.position", lightTransform->GetPosition());
		lightModelShader.setUniform("light.ambient", light->GetAmbient());
		lightModelShader.setUniform("light.diffuse", light->GetDiffuse());
		lightModelShader.setUniform("light.specular", light->GetSpecular());
		lightModelShader.setUniform("ViewPos", camera->position);
		lightModelShader.setUniformi("isGround", 1);
		lightModelShader.setUniformi("hasTexture", 1);
		// add shadows
		Entity* terrainEntity = dynamic_cast<Entity*>(terrain);
		GraphicsComponent* graphicsComponent = dynamic_cast<GraphicsComponent*>(terrainEntity->GetComponent("GraphicsComponent"));
		//add shadow
		mat4x4 trans = translate(mat4(), vec3(0.5f, 0.5f, 0.5f));
		mat4x4 sca = scale(mat4(), vec3(0.5f, 0.5f, 0.5f));
		mat4x4 B = trans*sca;

		mat4& viewMatrixLightPOV = glm::lookAt(lightTransform->GetPosition(), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		mat4x4 shadowMatrix = B*shadowObjet.shadowProjectionMatrix() * viewMatrixLightPOV;

		lightModelShader.setUniform("ShadowMatrix", shadowMatrix);
		lightModelShader.setUniformi("shadowMap", 1);
		graphicsComponent->setShadowsFbo(shadowObjet.shadowFbo());

		Material* material = graphicsComponent->GetMaterial();

		lightModelShader.setUniform("material.ambient", ambientColor);
		lightModelShader.setUniform("material.diffuse", material->GetDiffuse());
		lightModelShader.setUniform("material.specular", material->GetSpecular());
		lightModelShader.setUniformf("material.shininess", material->GetShinines());

		graphicsComponent->bindModelTextures();
		graphicsComponent->Draw(lightModelShader);
		lightModelShader.stop();
	}
}

void GraphicsSystem::RenderNormals()
{
	glm::mat4 view = camera->GetView();
	linesShader.start();
	linesShader.setUniform("ProjectionMatrix", projection);
	linesShader.setUniform("ViewMatrix", view);
	vector<Entity*>::iterator it2 = EntityManager::GetInstance()->GetEntities().begin();
	vector<Entity*>::iterator end2 = EntityManager::GetInstance()->GetEntities().end();
	for (; it2 != end2; ++it2)
	{

		GraphicsComponent* graphicsComponent = dynamic_cast<GraphicsComponent*>((*it2)->GetComponent("GraphicsComponent"));
		if (graphicsComponent)
		{

			TexturedModel* model = graphicsComponent->GetTexturedModel();
			glm::mat4 modelMatrix = graphicsComponent->GetModelMatrix();
			linesShader.setUniform("ModelMatrix", modelMatrix);
			model->DrawNormals();

			
			model->UnBind();
		}
		
		
	}
	linesShader.stop();
}

void GraphicsSystem::ShadowPass()
{
	shadowShader.start();
	shadowObjet.shadowFbo()->Bind();
	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glViewport(0, 0, 1024, 1024);
	

	vec3 up = vec3(0.0, 1.0, 0.0);
	TransformationComponent* lightTransform = dynamic_cast<TransformationComponent*>(light->GetComponent("TransformationComponent"));

	float near_plane = 0.1f, far_plane = 10000.0f;
	float lh = 0.6f*near_plane;
	float lw = lh * 1024 / 1024;
	glm::mat4 lightProjection = frustum(-lw, lw, -lh, lh, near_plane, far_plane);

	mat4x4 lightPOV = lookAt(lightTransform->GetPosition(), vec3(0, 0, 0), up);
	shadowShader.setUniform("ViewMatrix",lightPOV );
	shadowShader.setUniform("ProjectionMatrix", lightProjection);
	
	shadowObjet.setShadowProjectionMatrix(lightProjection);

	//Render Terrain
	Entity* terrainEntity = dynamic_cast<Entity*>(terrain);
	GraphicsComponent* terrainGraphicsComponent = dynamic_cast<GraphicsComponent*>(terrainEntity->GetComponent("GraphicsComponent"));
	terrainGraphicsComponent->Draw(shadowShader);
	
	//Render models
	vector<Entity*>::iterator it = EntityManager::GetInstance()->GetEntities().begin();
	vector<Entity*>::iterator end = EntityManager::GetInstance()->GetEntities().end();
	for (; it != end; ++it)
	{
		GraphicsComponent* graphicsComponent = dynamic_cast<GraphicsComponent*>((*it)->GetComponent("GraphicsComponent"));
		if (graphicsComponent)
		{
			graphicsComponent->Draw(shadowShader);
		}
	}

	
	shadowObjet.shadowFbo()->Unbind();
	shadowShader.stop();
	
}

void GraphicsSystem::LightModelPass()
{
	RenderTerrain();
	RenderEntities();
}

void GraphicsSystem::RenderSelectedVolumen()
{
	if (mouseSelectedEntity)
	{
			
			TransformationComponent* transformation = 
				dynamic_cast<TransformationComponent*>(mouseSelectedEntity->GetComponent(
				   "TransformationComponent"));

			BoundingSphere* boundingVolumen =
				dynamic_cast<BoundingSphere*>(mouseSelectedEntity->GetComponent(
					"BoundingVolumen"));

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			simpleShader.start();
			simpleShader.setUniform("ProjectionMatrix", projection);
			simpleShader.setUniform("ViewMatrix", camera->GetView());
			simpleShader.setUniform("ModelMatrix", 
				Math::CreateTransformationMatrix(transformation->GetPosition(), 
					quat(), vec3(boundingVolumen->GetRadius(), boundingVolumen->GetRadius(),
						boundingVolumen->GetRadius())));
			simpleShader.setUniform("LightColor", vec4(0.5, 0.0, 0.5, 0.4));
			boundingVolumen->GetModel()->Draw();
			simpleShader.stop();
			glDisable(GL_BLEND);
	}
	

}

bool GraphicsSystem::RayPlaneIntersection(vec3 planeNormal, vec3 rayStart, vec3 rayDirection)
{
	float denominator = dot(planeNormal, rayDirection);
	if (denominator == 0)
	{
		false;
	}
	return false;

}

void GraphicsSystem::RenderTextureToQuad(Fbo& frameBuffer)
{
	renderToQuadShader.setUniformi("screenTexture", 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render to the screen
	renderToQuadShader.start();
	glBindVertexArray(VAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, frameBuffer.texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	renderToQuadShader.stop();
	
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

Entity* GraphicsSystem::TestRayEntityIntersection(vec3 ray)
{

	EntityManager::EntityMap::iterator it =
		EntityManager::GetInstance()->GetEntities().begin();
	EntityManager::EntityMap::iterator end =
		EntityManager::GetInstance()->GetEntities().end();

	BoundingVolumen* boundingVolumen;

	for (;it != end; ++it)
	{
		Entity* entity = (*it);


		boundingVolumen = dynamic_cast<BoundingVolumen*>(entity->GetComponent(
			"BoundingVolumen"));

		if (boundingVolumen->RayInstersection(camera->position, ray))
		{
			return entity;
		}

		boundingVolumen = dynamic_cast<BoundingVolumen*>(light->GetComponent(
			"BoundingVolumen"));

		if (boundingVolumen->RayInstersection(camera->position, ray))
		{
			return light;
		}

		return nullptr;

	}
}


