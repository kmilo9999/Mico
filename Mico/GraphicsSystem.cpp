#include "GraphicsSystem.h"
#include <iostream>
#include <cassert>
#include <GLFW\glfw3.h>
#include "InputHandler.h"
#include "glm\glm.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "Math.h"
#include "WindowSystem.h"
#include "TransformationComponent.h"
#include "GraphicsComponent.h"
#include "FBO.h"
#include "GBuffer.h"

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

	delete light;
	delete terrain;
	delete hatchingTexture;
	delete gBuffer;
}

void GraphicsSystem::Init()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		getchar();
		assert(false && "Failed to Initlialize Opengl");
	}

	int major = 0;
	int minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	cout << "OpenGL minor version: " << minor << endl;
	cout << "OpenGL major version: " << major << endl;

	window = WINDOWSYSTEM->GetWindow();
	camera = new Camera(vec3(0, 12, 8), vec3(0, 0, -1));
	InputHandler::GetInstance()->addObserver(camera);
	InputHandler::GetInstance()->addObserver(this);

	lightModelShader.LoadShaders("LightModel.vertexshader", "LightModel.fragmentshader");
	simpleShader.LoadShaders("Simple.vertexshader", "Simple.fragmentshader");
	terrainShader.LoadShaders("Terrain.vertexshader", "Terrain.fragmentshader");
	//normalRenderShader.LoadShaders("NormalLines.vertexshader", "NormalLines.fragmentshader");
	normalRenderShader.LoadShaders("NormalLines.vertexshader", "NormalLines.fragmentshader", "NormalLines.geometryshader");
	shadowShader.LoadShaders("Shadows.vertexshader", "Shadows.fragmentshader");
	//hatchingShader.LoadShaders("Hatching.vertexshader", "Hatching.fragmentshader");
	hatchingShader.LoadShaders("Hatching.vertexshader", "NEWHatching.fragmentshader", "Hatching.geometryshader");
	newLightModelShader.LoadShaders("NewLightModel.vertexshader", "NewLightModel.fragmentshader", "NewLightModel.geometryshader");
	celShadingShader.LoadShaders("CelShading.vertexshader", "CelShading.fragmentshader");
	goochShadingShader.LoadShaders("GoochShading.vertexshader", "GoochShading.fragmentshader");
	edgeDetectShader.LoadShaders("EdgeDetector.vertexshader", "EdgeDetector.fragmentshader", "EdgeDetector.geometryshader");
	gBufferShader.LoadShaders("gbuffer.vertexshader","gbuffer.fragmentshader");


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

	simpleShader.addUniform("ProjectionMatrix");
	simpleShader.addUniform("ViewMatrix");
	simpleShader.addUniform("ModelMatrix");
	simpleShader.addUniform("LightColor");

	terrainShader.addUniform("ProjectionMatrix");
	terrainShader.addUniform("ViewMatrix");
	terrainShader.addUniform("ModelMatrix");
	terrainShader.addUniform("shadowMap");
	terrainShader.addUniform("ShadowMatrix");

	normalRenderShader.addUniform("ProjectionMatrix");
	normalRenderShader.addUniform("ViewMatrix");
	normalRenderShader.addUniform("ModelMatrix");
	normalRenderShader.addUniform("ViewPos");
	normalRenderShader.addUniform("ViewInverse");

	Fbo* shadowframeBuffer = new Fbo();
	//shadowframeBuffer->CreateFBO(1024, 1024);
	shadowframeBuffer->CreateFBO(windowSize.x, windowSize.y);

	shadowObjet.setShadowFbo(shadowframeBuffer);

	shadowShader.addUniform("ProjectionMatrix");
	shadowShader.addUniform("ViewMatrix");
	shadowShader.addUniform("ModelMatrix");

	renderToQuadShader.LoadShaders("quad.vertexshader", "quad.fragmentshader");
	renderToQuadShader.addUniform("positionMap");
	renderToQuadShader.addUniform("normalMap");
	renderToQuadShader.addUniform("textureMap");
	renderToQuadShader.addUniform("diffuseMap");
	renderToQuadShader.addUniform("specularMap");
	renderToQuadShader.addUniform("shininessMap");
	//renderToQuadShader.addUniform("depthMap");
	renderToQuadShader.addUniform("shadowMap");
	renderToQuadShader.addUniform("light.position");
	renderToQuadShader.addUniform("light.ambient");
	renderToQuadShader.addUniform("light.diffuse");
	renderToQuadShader.addUniform("light.specular");
	renderToQuadShader.addUniform("ShadowMatrix");
	renderToQuadShader.addUniform("ViewPos");
	renderToQuadShader.addUniform("width");
	renderToQuadShader.addUniform("height");


	hatchingShader.addUniform("ProjectionMatrix");
	hatchingShader.addUniform("ViewMatrix");
	hatchingShader.addUniform("ModelMatrix");
	hatchingShader.addUniform("light.position");
	hatchingShader.addUniform("light.ambient");
	hatchingShader.addUniform("light.diffuse");
	hatchingShader.addUniform("light.specular");
	hatchingShader.addUniform("ViewPos");
	hatchingShader.addUniform("isGround");


	newLightModelShader.addUniform("ProjectionMatrix");
	newLightModelShader.addUniform("ViewMatrix");
	newLightModelShader.addUniform("ModelMatrix");
	newLightModelShader.addUniform("light.position");
	newLightModelShader.addUniform("light.ambient");
	newLightModelShader.addUniform("light.diffuse");
	newLightModelShader.addUniform("light.specular");
	newLightModelShader.addUniform("material.ambient");
	newLightModelShader.addUniform("material.diffuse");
	newLightModelShader.addUniform("material.specular");
	newLightModelShader.addUniform("material.shininess");
	newLightModelShader.addUniform("ViewPos");
	newLightModelShader.addUniform("hasTexture");
	newLightModelShader.addUniform("isGround");


	celShadingShader.addUniform("ProjectionMatrix");
	celShadingShader.addUniform("ViewMatrix");
	celShadingShader.addUniform("ModelMatrix");
	celShadingShader.addUniform("light.position");
	celShadingShader.addUniform("light.ambient");
	celShadingShader.addUniform("light.diffuse");
	celShadingShader.addUniform("light.specular");
	celShadingShader.addUniform("material.ambient");
	celShadingShader.addUniform("material.diffuse");
	celShadingShader.addUniform("material.specular");
	celShadingShader.addUniform("material.shininess");
	celShadingShader.addUniform("ViewPos");
	celShadingShader.addUniform("numShades");

	
	goochShadingShader.addUniform("ProjectionMatrix");
	goochShadingShader.addUniform("ViewMatrix");
	goochShadingShader.addUniform("ModelMatrix");
	goochShadingShader.addUniform("light.position");
	goochShadingShader.addUniform("light.ambient");
	goochShadingShader.addUniform("light.diffuse");
	goochShadingShader.addUniform("light.specular");
	goochShadingShader.addUniform("material.ambient");
	goochShadingShader.addUniform("material.diffuse");
	goochShadingShader.addUniform("material.specular");
	goochShadingShader.addUniform("material.shininess");
	goochShadingShader.addUniform("ViewPos");
	goochShadingShader.addUniform("coolColor");
	goochShadingShader.addUniform("warmColor");
	goochShadingShader.addUniform("alpha");
	goochShadingShader.addUniform("beta");

    edgeDetectShader.addUniform("ProjectionMatrix");
	edgeDetectShader.addUniform("ViewMatrix");
	edgeDetectShader.addUniform("ModelMatrix");
	edgeDetectShader.addUniform("light.position");
	edgeDetectShader.addUniform("light.ambient");
	edgeDetectShader.addUniform("light.diffuse");
	edgeDetectShader.addUniform("light.specular");
	edgeDetectShader.addUniform("material.ambient");
	edgeDetectShader.addUniform("material.diffuse");
	edgeDetectShader.addUniform("material.specular");
	edgeDetectShader.addUniform("material.shininess");
	edgeDetectShader.addUniform("ViewPos");
	edgeDetectShader.addUniform("ViewInverse");
	edgeDetectShader.addUniform("showModel");

	gBuffer = new GBuffer();
	gBuffer->CreateGBuffer(windowSize.x,windowSize.y);

	gBufferShader.addUniform("ProjectionMatrix");
	gBufferShader.addUniform("ViewMatrix");
	gBufferShader.addUniform("ModelMatrix");
	gBufferShader.addUniform("useTexture");
	gBufferShader.addUniform("material.diffuse");
	gBufferShader.addUniform("material.specular");
	gBufferShader.addUniform("material.shininess");
	


	// Load 3D texture
	vector<string> texturesVector;
	/*texturesVector.push_back("../Resources/Textures/hatching/hatching2_1.jpg");
	texturesVector.push_back("../Resources/Textures/hatching/hatching2_2.jpg");
	texturesVector.push_back("../Resources/Textures/hatching/hatching2_3.jpg");
	texturesVector.push_back("../Resources/Textures/hatching/hatching2_4.jpg");
	texturesVector.push_back("../Resources/Textures/hatching/hatching2_5.jpg");
	texturesVector.push_back("../Resources/Textures/hatching/hatching2_6.jpg");*/
	
	
	texturesVector.push_back("../Resources/Textures/new_hatching/hatching7.jpg");
	texturesVector.push_back("../Resources/Textures/new_hatching/hatching6.jpg");
	texturesVector.push_back("../Resources/Textures/new_hatching/hatching5.jpg");
	texturesVector.push_back("../Resources/Textures/new_hatching/hatching4.jpg");
	texturesVector.push_back("../Resources/Textures/new_hatching/hatching3.jpg");
	texturesVector.push_back("../Resources/Textures/new_hatching/hatching2.jpg");
	texturesVector.push_back("../Resources/Textures/new_hatching/hatching1.jpg");
	texturesVector.push_back("../Resources/Textures/new_hatching/hatching0.jpg");
	
	
	

	hatchingTexture = new Texture(GL_TEXTURE_2D_ARRAY, texturesVector);


	

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

void GraphicsSystem::Update(float dt)
{

	camera->Update();
	projection = glm::perspective(camera->foV, windowSize.x / windowSize.y, 0.1f, 1000.0f);
	light->Update();
	if (terrain)
	{
		terrain->Update();
	}
	

	//ShadowPass();



	InitRender();
	//RenderTextureToQuad(*shadowObjet.shadowFbo());

	//RenderTerrain();
	//LightModelPass();
	DefferedShadingRender();
	//HatchingPass();
	
	//NewLightModelPass(); // Adjacent triangles
	//CelShadingPass();
	//GoochShadingPass();

	//EdgeDetectionPass();

	/*if (showNormals)
	{
		RenderNormals();
	}*/

	//RenderGlobalLight();
	
	//RenderSelectedVolumen();

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
			if (mouseSelectedEntity)
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

			if (deltaX && deltaX < 0 && absolutX > 2.5)
			{
				newPosition -= rightVector;
			}
			else if (deltaX && deltaX > 0 && absolutX > 2.5)
			{
				newPosition += rightVector;
			}

			if (deltaY && deltaY < 0 && absolutY > 2.5)
			{
				newPosition += upvector;
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
		case Event::ShowModel:
			showModel = evt.ui.value.boolValue;
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

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void GraphicsSystem::InitScene()
{
	light = new GlobalLight();

	light->SetAmbient(vec3(0.5f));
	light->SetDiffuse(vec3(0.5));
	light->SetSpecular(vec3(1.0f));

	TransformationComponent* lightTransformation =
		new TransformationComponent(vec3(1.0f, 15.0f, 5.3f), quat(), vec3(0.3f, 0.3f, 0.3f));
	light->AddComponent(lightTransformation);

	Texture* targetTtexture = new Texture(GL_TEXTURE_2D, "../Resources/Textures/texture_sample.jpg");
	Material* material = new Material(vec3(1.0f, 0.5f, 0.31f), vec3(1.0f, 0.5f, 0.31f), vec3(0.2f, 0.2f, 0.2f), 64.0f);
	material->AddTexture(targetTtexture);
	GraphicsComponent* graphics = new GraphicsComponent(EntityManager::GetInstance()->GetModelByName("cube"), material);
	light->AddComponent(graphics);

	SelectableBoundingVolumen* boundingVolumen = new BoundingSphere(0.7f);
	boundingVolumen->SetModel(EntityManager::GetInstance()->GetModelByName("cube"));
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
	if (terrain)
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
			GraphicsComponent* terrainGraphicsComponent = dynamic_cast<GraphicsComponent*>(terrainEntity->GetComponent("GraphicsComponent"));
			//add shadow
			mat4x4 trans = translate(mat4(), vec3(0.5f, 0.5f, 0.5f));
			mat4x4 sca = scale(mat4(), vec3(0.5f, 0.5f, 0.5f));
			mat4x4 B = trans*sca;

			mat4& viewMatrixLightPOV = glm::lookAt(lightTransform->GetPosition(), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
			mat4x4 shadowMatrix = B*shadowObjet.shadowProjectionMatrix() * viewMatrixLightPOV;

			lightModelShader.setUniform("ShadowMatrix", shadowMatrix);
			lightModelShader.setUniformi("shadowMap", 1);
			terrainGraphicsComponent->setShadowsFbo(shadowObjet.shadowFbo());

			Material* material = terrainGraphicsComponent->GetMaterial();

			lightModelShader.setUniform("material.ambient", ambientColor);
			lightModelShader.setUniform("material.diffuse", material->GetDiffuse());
			lightModelShader.setUniform("material.specular", material->GetSpecular());
			lightModelShader.setUniformf("material.shininess", material->GetShinines());

			terrainGraphicsComponent->bindModelTextures();
			terrainGraphicsComponent->Draw(lightModelShader);
			lightModelShader.stop();
		}
	}
	
}

void GraphicsSystem::RenderNormals()
{
	glm::mat4 view = camera->GetView();
	normalRenderShader.start();
	normalRenderShader.setUniform("ProjectionMatrix", projection);
	normalRenderShader.setUniform("ViewMatrix", view);
	normalRenderShader.setUniform("ViewPos", camera->position);
	
	mat4 viewM = camera->GetView();
	mat4x4 WorldInv = affineInverse(viewM);
	normalRenderShader.setUniform("ViewInverse", WorldInv);

	vector<Entity*>::iterator it = EntityManager::GetInstance()->GetEntities().begin();
	vector<Entity*>::iterator end = EntityManager::GetInstance()->GetEntities().end();
	for (; it != end; ++it)
	{
		GraphicsComponent* graphicsComponent = dynamic_cast<GraphicsComponent*>((*it)->GetComponent("GraphicsComponent"));
		if (graphicsComponent)
		{
			graphicsComponent->Draw(normalRenderShader);
		}
	}
	normalRenderShader.stop();
}

void GraphicsSystem::ShadowPass()
{
	shadowShader.start();
	shadowObjet.shadowFbo()->Bind();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glViewport(0, 0, 1024, 1024);
	glViewport(0, 0, windowSize.x, windowSize.y);


	vec3 up = vec3(0.0, 1.0, 0.0);
	TransformationComponent* lightTransform = dynamic_cast<TransformationComponent*>(light->GetComponent("TransformationComponent"));

	float near_plane = 0.1f, far_plane = 10000.0f;
	float lh = 0.6f*near_plane;
	//float lw = lh * 1024 / 1024;
	float lw = lh * windowSize.x / windowSize.y;
	glm::mat4 lightProjection = frustum(-lw, lw, -lh, lh, near_plane, far_plane);

	mat4x4 lightPOV = lookAt(lightTransform->GetPosition(), vec3(0, 0, 0), up);
	shadowShader.setUniform("ViewMatrix", lightPOV);
	shadowShader.setUniform("ProjectionMatrix", lightProjection);

	shadowObjet.setShadowProjectionMatrix(lightProjection);

	//Render Terrain
	if (terrain)
	{
		Entity* terrainEntity = dynamic_cast<Entity*>(terrain);
		GraphicsComponent* terrainGraphicsComponent = dynamic_cast<GraphicsComponent*>(terrainEntity->GetComponent("GraphicsComponent"));
		terrainGraphicsComponent->Draw(shadowShader);
	}

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
	RenderEntities();
}

void GraphicsSystem::HatchingPass()
{
	//light pass
	TransformationComponent* lightTransform = dynamic_cast<TransformationComponent*>(light->GetComponent("TransformationComponent"));

	if (lightTransform)
	{
		hatchingShader.start();
		hatchingShader.setUniform("ProjectionMatrix", projection);
		hatchingShader.setUniform("ViewMatrix", camera->GetView());
		hatchingShader.setUniform("light.position", lightTransform->GetPosition());
		hatchingShader.setUniform("light.ambient", light->GetAmbient());
		hatchingShader.setUniform("light.diffuse", light->GetDiffuse());
		hatchingShader.setUniform("light.specular", light->GetSpecular());
		hatchingShader.setUniform("ViewPos", camera->position);

		
		if (terrain)
		{
			Entity* terrainEntity = dynamic_cast<Entity*>(terrain);
			GraphicsComponent* terrainGraphicsComponent = dynamic_cast<GraphicsComponent*>(terrainEntity->GetComponent("GraphicsComponent"));

			//! Fetch the 3D Texture
			hatchingTexture->Bind(0);
			hatchingShader.setUniformi("textureArray", 0);


			Material* material = terrainGraphicsComponent->GetMaterial();
			hatchingShader.setUniform("material.ambient", ambientColor);
			hatchingShader.setUniform("material.diffuse", material->GetDiffuse());
			hatchingShader.setUniform("material.specular", material->GetSpecular());
			hatchingShader.setUniformf("material.shininess", material->GetShinines());

			terrainGraphicsComponent->Draw(hatchingShader);
		}
	

		vector<Entity*>::iterator it = EntityManager::GetInstance()->GetEntities().begin();
		vector<Entity*>::iterator end = EntityManager::GetInstance()->GetEntities().end();
		for (; it != end; ++it)
		{
			GraphicsComponent* entityGraphicsComponent = dynamic_cast<GraphicsComponent*>((*it)->GetComponent("GraphicsComponent"));
			if (entityGraphicsComponent)
			{
				Material* material = entityGraphicsComponent->GetMaterial();
				hatchingShader.setUniform("material.ambient", ambientColor);
				hatchingShader.setUniform("material.diffuse", material->GetDiffuse());
				hatchingShader.setUniform("material.specular", material->GetSpecular());
				hatchingShader.setUniformf("material.shininess", material->GetShinines());
				/*if(entityGraphicsComponent->GetTexturedModel()->GetId().compare("plane_subdivided2")==0)
				{
					hatchingShader.setUniformi("isGround",1);
				}
				else
				{
					hatchingShader.setUniformi("isGround", 0);
				}*/

				//! Fetch the 3D Texture
				hatchingTexture->Bind(0);
				hatchingShader.setUniformi("textureArray", 0);
				entityGraphicsComponent->Draw(hatchingShader);
			}
		}

		hatchingShader.stop();
	}

}

void GraphicsSystem::CelShadingPass()
{
	//light pass
	TransformationComponent* lightTransform = dynamic_cast<TransformationComponent*>(light->GetComponent("TransformationComponent"));

	if (lightTransform)
	{
		celShadingShader.start();
		celShadingShader.setUniform("ProjectionMatrix", projection);
		celShadingShader.setUniform("ViewMatrix", camera->GetView());
		celShadingShader.setUniform("light.position", lightTransform->GetPosition());
		celShadingShader.setUniform("light.ambient", light->GetAmbient());
		celShadingShader.setUniform("light.diffuse", light->GetDiffuse());
		celShadingShader.setUniform("light.specular", light->GetSpecular());
		celShadingShader.setUniform("ViewPos", camera->position);


		vector<Entity*>::iterator it = EntityManager::GetInstance()->GetEntities().begin();
		vector<Entity*>::iterator end = EntityManager::GetInstance()->GetEntities().end();
		for (; it != end; ++it)
		{
			GraphicsComponent* entityGraphicsComponent = dynamic_cast<GraphicsComponent*>((*it)->GetComponent("GraphicsComponent"));
			if (entityGraphicsComponent)
			{
				Material* material = entityGraphicsComponent->GetMaterial();
				celShadingShader.setUniform("material.ambient", ambientColor);
				celShadingShader.setUniform("material.diffuse", material->GetDiffuse());
				celShadingShader.setUniform("material.specular", material->GetSpecular());
				celShadingShader.setUniformf("material.shininess", material->GetShinines());
				celShadingShader.setUniformi("numShades", material->GetBrighLevel());
				entityGraphicsComponent->Draw(celShadingShader);
			}
		}

		celShadingShader.stop();
	}
}

void GraphicsSystem::GoochShadingPass()
{
	//light pass
	TransformationComponent* lightTransform = dynamic_cast<TransformationComponent*>(light->GetComponent("TransformationComponent"));

	if (lightTransform)
	{
		goochShadingShader.start();
		goochShadingShader.setUniform("ProjectionMatrix", projection);
		goochShadingShader.setUniform("ViewMatrix", camera->GetView());
		goochShadingShader.setUniform("light.position", lightTransform->GetPosition());
		goochShadingShader.setUniform("light.ambient", light->GetAmbient());
		goochShadingShader.setUniform("light.diffuse", light->GetDiffuse());
		goochShadingShader.setUniform("light.specular", light->GetSpecular());
		goochShadingShader.setUniform("ViewPos", camera->position);


		vector<Entity*>::iterator it = EntityManager::GetInstance()->GetEntities().begin();
		vector<Entity*>::iterator end = EntityManager::GetInstance()->GetEntities().end();
		for (; it != end; ++it)
		{
			GraphicsComponent* entityGraphicsComponent = dynamic_cast<GraphicsComponent*>((*it)->GetComponent("GraphicsComponent"));
			if (entityGraphicsComponent)
			{
				Material* material = entityGraphicsComponent->GetMaterial();
				goochShadingShader.setUniform("material.ambient", ambientColor);
				goochShadingShader.setUniform("material.diffuse", material->GetDiffuse());
				goochShadingShader.setUniform("material.specular", material->GetSpecular());
				goochShadingShader.setUniformf("material.shininess", material->GetShinines());
				goochShadingShader.setUniform("coolColor", material->GetColdColor());
				goochShadingShader.setUniform("warmColor", material->GetWarmColor());
				goochShadingShader.setUniformf("alpha", material->GetAlpha());
				goochShadingShader.setUniformf("beta", material->GetBeta());

				entityGraphicsComponent->Draw(goochShadingShader);
			}
		}

		goochShadingShader.stop();
	}
}

void GraphicsSystem::EdgeDetectionPass()
{
	//light pass
	TransformationComponent* lightTransform = dynamic_cast<TransformationComponent*>(light->GetComponent("TransformationComponent"));

	if (lightTransform)
	{
		edgeDetectShader.start();
		edgeDetectShader.setUniform("ProjectionMatrix", projection);
		edgeDetectShader.setUniform("ViewMatrix", camera->GetView());
		edgeDetectShader.setUniform("light.position", lightTransform->GetPosition());
		edgeDetectShader.setUniform("light.ambient", light->GetAmbient());
		edgeDetectShader.setUniform("light.diffuse", light->GetDiffuse());
		edgeDetectShader.setUniform("light.specular", light->GetSpecular());	
		edgeDetectShader.setUniform("ViewPos", camera->position);
		edgeDetectShader.setUniformi("showModel", showModel);

		mat4 viewM = camera->GetView();
		mat4x4 WorldInv = affineInverse(viewM);
		edgeDetectShader.setUniform("ViewInverse", WorldInv);


		vector<Entity*>::iterator it = EntityManager::GetInstance()->GetEntities().begin();
		vector<Entity*>::iterator end = EntityManager::GetInstance()->GetEntities().end();
		for (; it != end; ++it)
		{
			GraphicsComponent* entityGraphicsComponent = dynamic_cast<GraphicsComponent*>((*it)->GetComponent("GraphicsComponent"));
			if (entityGraphicsComponent)
			{
				Material* material = entityGraphicsComponent->GetMaterial();
				edgeDetectShader.setUniform("material.ambient", ambientColor);
				edgeDetectShader.setUniform("material.diffuse", material->GetDiffuse());
				edgeDetectShader.setUniform("material.specular", material->GetSpecular());
				edgeDetectShader.setUniformf("material.shininess", material->GetShinines());
				entityGraphicsComponent->Draw(edgeDetectShader);
			}
		}

		edgeDetectShader.stop();
	}
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

void GraphicsSystem::NewLightModelPass()
{
	TransformationComponent* lightTransform = dynamic_cast<TransformationComponent*>(light->GetComponent("TransformationComponent"));
	if (lightTransform)
	{

		newLightModelShader.start();
		newLightModelShader.setUniform("ProjectionMatrix", projection);
		newLightModelShader.setUniform("ViewMatrix", camera->GetView());
		newLightModelShader.setUniform("light.position", lightTransform->GetPosition());
		newLightModelShader.setUniform("light.ambient", light->GetAmbient());
		newLightModelShader.setUniform("light.diffuse", light->GetDiffuse());
		newLightModelShader.setUniform("light.specular", light->GetSpecular());
		newLightModelShader.setUniform("ViewPos", camera->position);
		newLightModelShader.setUniformi("isGround", 0);

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
					newLightModelShader.setUniformi("hasTexture", 1);
				}
				else
				{
					newLightModelShader.setUniformi("hasTexture", 0);
				}

				newLightModelShader.setUniform("material.ambient", ambientColor);
				newLightModelShader.setUniform("material.diffuse", material->GetDiffuse());
				newLightModelShader.setUniform("material.specular", material->GetSpecular());
				newLightModelShader.setUniformf("material.shininess", material->GetShinines());
				graphicsComponent->bindModelTextures();
				graphicsComponent->Draw(newLightModelShader);
			}


		}
		newLightModelShader.stop();
	}
}

void GraphicsSystem::DefferedShadingRender()
{
	ShadowPass();

	// Geometry pass
	gBuffer->BindForWriting();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);


	gBufferShader.start();


	gBufferShader.setUniform("ProjectionMatrix", projection);
	gBufferShader.setUniform("ViewMatrix", camera->GetView());

	if (terrain)
	{
		Entity* terrainEntity = dynamic_cast<Entity*>(terrain);
		GraphicsComponent* terrainGraphicsComponent = dynamic_cast<GraphicsComponent*>(terrainEntity->GetComponent("GraphicsComponent"));

		Material* material = terrainGraphicsComponent->GetMaterial();
		gBufferShader.setUniformi("useTexture", material->GetTextures().size() > 0 ? 1 : 0);
		//lightModelShader.setUniform("material.ambient", ambientColor);
		//lightModelShader.setUniform("material.diffuse", material->GetDiffuse());
		gBufferShader.setUniform("material.specular", material->GetSpecular());
		gBufferShader.setUniformf("material.shininess", material->GetShinines());

		terrainGraphicsComponent->bindModelTextures();
		terrainGraphicsComponent->Draw(gBufferShader);
	}

	vector<Entity*>::iterator it = EntityManager::GetInstance()->GetEntities().begin();
	vector<Entity*>::iterator end = EntityManager::GetInstance()->GetEntities().end();
	for (; it != end; ++it)
	{

		GraphicsComponent* graphicsComponent = dynamic_cast<GraphicsComponent*>((*it)->GetComponent("GraphicsComponent"));
		if (graphicsComponent)
		{
			Material* material = graphicsComponent->GetMaterial();
			gBufferShader.setUniformi("useTexture", material->GetTextures().size() > 0 ? 1 : 0);
			//gBufferShader.setUniform("material.ambient", ambientColor);
			//gBufferShader.setUniform("material.diffuse", material->GetDiffuse());
			gBufferShader.setUniform("material.specular", material->GetSpecular());
			gBufferShader.setUniformf("material.shininess", material->GetShinines());
			graphicsComponent->Draw(gBufferShader);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	gBufferShader.stop();


	//add shadow
	mat4x4 trans = translate(mat4(), vec3(0.5f, 0.5f, 0.5f));
	mat4x4 sca = scale(mat4(), vec3(0.5f, 0.5f, 0.5f));
	mat4x4 B = trans*sca;

	TransformationComponent* lightTransform = dynamic_cast<TransformationComponent*>(light->GetComponent("TransformationComponent"));

	mat4& viewMatrixLightPOV = glm::lookAt(lightTransform->GetPosition(), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	mat4x4 shadowMatrix = B*shadowObjet.shadowProjectionMatrix() * viewMatrixLightPOV;


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderToQuadShader.start();
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glBindVertexArray(VAO);
	glDisable(GL_DEPTH_TEST);
	
	if (lightTransform)
	{
		renderToQuadShader.setUniform("ViewPos", camera->position);
		//renderToQuadShader.setUniformi("isGround", 0);
		renderToQuadShader.setUniform("ShadowMatrix", shadowMatrix);

		renderToQuadShader.setUniform("ambient", ambientColor);

		renderToQuadShader.setUniform("light.position", lightTransform->GetPosition());
		renderToQuadShader.setUniform("light.ambient", light->GetAmbient());
		renderToQuadShader.setUniform("light.diffuse", light->GetDiffuse());
		renderToQuadShader.setUniform("light.specular", light->GetSpecular());
		renderToQuadShader.setUniformf("width", windowSize.x);
		renderToQuadShader.setUniformf("height", windowSize.y);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gBuffer->GetTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION));
		renderToQuadShader.setUniformi("positionMap", GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);

		glActiveTexture(GL_TEXTURE0 + GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
		glBindTexture(GL_TEXTURE_2D, gBuffer->GetTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL));
		renderToQuadShader.setUniformi("normalMap", GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);

		glActiveTexture(GL_TEXTURE0 + GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
		glBindTexture(GL_TEXTURE_2D, gBuffer->GetTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE));
		renderToQuadShader.setUniformi("diffuseMap", GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);

		glActiveTexture(GL_TEXTURE0 + GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR);
		glBindTexture(GL_TEXTURE_2D, gBuffer->GetTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR));
		renderToQuadShader.setUniformi("specularMap", GBuffer::GBUFFER_TEXTURE_TYPE_SPECULAR);

		glActiveTexture(GL_TEXTURE0 + GBuffer::GBUFFER_TEXTURE_TYPE_SHININESMAP);
		glBindTexture(GL_TEXTURE_2D, gBuffer->GetTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_SHININESMAP));
		renderToQuadShader.setUniformi("shininessMap", GBuffer::GBUFFER_TEXTURE_TYPE_SHININESMAP);

		glActiveTexture(GL_TEXTURE0 + GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
		glBindTexture(GL_TEXTURE_2D, gBuffer->GetTextureUnit(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD));
		renderToQuadShader.setUniformi("textureMap", GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);

		glActiveTexture(GL_TEXTURE0 + GBuffer::GBUFFER_NUM_TEXTURES);
		glBindTexture(GL_TEXTURE_2D, shadowObjet.shadowFbo()->texture);
		renderToQuadShader.setUniformi("shadowMap", GBuffer::GBUFFER_NUM_TEXTURES);

		/*glActiveTexture(GL_TEXTURE0 + GBuffer::GBUFFER_NUM_TEXTURES + 1);
		glBindTexture(GL_TEXTURE_2D, gBuffer->GetDepthTexture());
		renderToQuadShader.setUniformi("depthMap", GBuffer::GBUFFER_NUM_TEXTURES);*/

		glDrawArrays(GL_TRIANGLES, 0, 6);
		renderToQuadShader.stop();

		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ZERO);
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
	mat4 view = camera->GetView();
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


