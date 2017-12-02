#include "EntityManager.h"
#include "Loader.h"
#include "SelectableBoundingVolumen.h"
#include "Event.h"
#include "TransformationComponent.h"
#include "GraphicsComponent.h"
#include <iostream>
#include <glm/gtx/quaternion.hpp>

EntityManager* EntityManager::instance(0);

//vector<vec3> vertices{  vec3(1.0f,-1.0f,-1.0f),
//						vec3(1.0f,-1.0f,1.0f),
//						vec3(-1.0f,-1.0f,1.0f),
//						vec3(-1.0f,-1.0f,-1.0f),
//						vec3(1.0f,1.0f,-1.0f),
//						vec3(1.0f,1.0f,1.0f),
//						vec3(-1.0f,1.0f,1.0f),
//						vec3(-1.0f,1.0f,-1.0f) };
//
//vector<vec2> textureCoordinates = { vec2(0.333333,0.666667),
//						vec2(0.333333,1.000000),
//						vec2(0.000000, 1.000000),
//						vec2(0.000000,0.666667),
//						vec2(0.000000,0.333333),
//						vec2(0.333333,0.333333),
//						vec2(0.333333,0.000000),
//						vec2(0.666667,0.000000),
//						vec2(0.000000,0.000000),
//						vec2(1.000000,0.333333),
//						vec2(0.666667,0.333333),
//						vec2(0.666667,0.666667),
//						vec2(1.000000,0.000000)
//};
//
//vector<vec3> normals = { vec3(0.000000,-1.000000,0.000000),
//						 vec3(0.000000,1.000000,0.000000),
//						 vec3(1.000000,0.000000,0.000000),
//						 vec3(-0.000000,0.000000,1.000000),
//						 vec3(-1.000000,-0.000000,-0.000000),
//						 vec3(0.000000,0.000000,-1.000000)
//};
//
//vector<int> indexes{ 2,3,4,
//					 0,3,2 };

vector<vec2> textcoor{
	vec2(1.0f, 1.0f), // Top Right
	vec2(1.0f, 0.0f), // Bottom Right
	vec2(0.0f, 0.0f), // Bottom Left
	vec2(0.0f, 1.0f)  // Top Left 
};

EntityManager::EntityManager()
	: myCurrentCenterModel(MODEL::Bunny)
{

}



EntityManager * EntityManager::GetInstance()
{
	if (!instance)
	{
		instance = new EntityManager();
	}
	return instance;
}

EntityManager::EntityMap & EntityManager::GetEntities()
{
	return myEntities;
}

void EntityManager::UpdateEntities()
{
	EntityMap::iterator it = myEntities.begin();
	for (;it != myEntities.end(); ++it)
	{
		(*it)->Update();
	}
}

TexturedModel * EntityManager::GetModelByName(string modelId)
{
	ModelMap::iterator it = myModels.find(modelId);
	if (it != myModels.end())
	{
		return it->second;
	}
	else
	{
		return GetModel(modelId, GL_TRIANGLES);
	}
	
}

TexturedModel * EntityManager::GetModelByNameAndMode(string modelId, GLenum mode)
{
	ModelMap::iterator it = myModels.find(modelId);
	if (it != myModels.end())
	{
		return it->second;
	}
	else
	{
		return GetModel( modelId, mode);
	}
}

TexturedModel * EntityManager::GetModel(string modelId, GLenum mode)
{
	//load the model
	string filePath = Loader::GetInstance()->modelFilePath();
	string modelFilePath = filePath.append(modelId).append(".obj");
	TexturedModel* model =
		Loader::GetInstance()->LoadToFromFile(modelFilePath.c_str(), mode);
	if (model != nullptr)
	{
		model->SetId(modelId);
		InsertModelEntities(model);
		std::cout << modelFilePath << " loaded" << std::endl;
		return model;
	}

	return nullptr;
}

EntityManager::~EntityManager()
{
	for (int i = 0 ; i < myEntities.size(); ++i)
	{ 
		delete myEntities[i];
		
	}

	myEntities.clear();

	if (instance)
	{
		delete instance;
	}
}

void EntityManager::ReplaceCenterModel(string modelId)
{
	TexturedModel* replaceModel (GetModelByName(modelId));
	GraphicsComponent* graphicsComponent = dynamic_cast<GraphicsComponent*>
		(myCenterEntity->GetComponent("GraphicsComponent"));
	graphicsComponent->SetTexturedModel(replaceModel);
}

void EntityManager::Initialize()
{
	
	//Initialize center model
	//CreateEntity("bunny", vec3(0.0f, 11.0f, 3.0f), quat(), vec3(1.0f, 1.0f, 1.0f), GL_TRIANGLES);
	CreateEntity("teapot", vec3(0.0f, 11.0f, 3.0f), quat(), vec3(1.0f, 1.0f, 1.0f), GL_TRIANGLES_ADJACENCY);
	myCenterModelId = "teapot";

	//CreateEntity("new_sphere", vec3(3.0f, 12.0f, 3.0f), quat(), vec3(1.0f, 1.0f, 1.0f), GL_TRIANGLES_ADJACENCY);

	

	//CreateEntity("plane_subdivided2", vec3(3.0f, 11.0f, 3.0f), quat(), vec3(10.0f, 1.0f, 10.0f), GL_TRIANGLES_ADJACENCY);
	//myCenterModelId = "teapot";

	//CreateEntity("bunny", vec3(0.0f, 11.0f, 3.0f), quat(), vec3(1.0f, 1.0f, 1.0f), GL_TRIANGLES_ADJACENCY);
	//myCenterModelId = "bunny";



	/*Entity* targetPlane = new Entity();
	targetPlane->SetPosition(vec3(0.0f, 10.0f, 0.0f));
	targetPlane->SetScale(vec3(1.0f, 1.0f, 1.0f));*/
	
	//InserModelEntity("plane", targetPlane);

	EntityMap::iterator it = myEntities.begin();
	for (; it != myEntities.end(); ++it)
	{
		(*it)->Initialize();
	}
}


void EntityManager::InsertModelEntities(TexturedModel * model)
{
	myModels.insert(std::make_pair(model->GetId(), model));
}

void EntityManager::InserModelEntity( Entity * entity)
{
	myEntities.push_back(entity);
}

void EntityManager::RemoveEntity( unsigned int entityId)
{
	EntityMap::iterator it = myEntities.begin();
	for (; it != myEntities.end(); ++it)
	{
		if ((*it)->EntityId == entityId)
		{
			myEntities.erase(it);
			return;
		}

	}
	
}

void EntityManager::CreateEntity(string modelId, vec3 postion, quat orientation, vec3 scale, GLenum mode)
{
	myCenterEntity = new Entity();
	TransformationComponent* transformation = new TransformationComponent(postion, orientation, scale);
	myCenterEntity->AddComponent(transformation);
	
	//Texture* targetTtexture = new Texture(GL_TEXTURE_2D, "../Resources/Textures/texture_sample.jpg");
	Material* material = new Material(vec3(1.0f, 0.5f, 0.31f), vec3(1.0f, 0.5f, 0.31f), vec3(0.5f, 0.5f, 0.5f), 64.0f);
	//material->AddTexture(targetTtexture);
	GraphicsComponent* graphics = new GraphicsComponent(GetModelByNameAndMode(modelId,mode), material);
	myCenterEntity->AddComponent(graphics);

    SelectableBoundingVolumen* boundingVolumen = new BoundingSphere(0.7f);
	boundingVolumen->SetModel(GetModelByName("sphere"));
	myCenterEntity->AddComponent(boundingVolumen);

	myCenterEntity->SetEntityId(myCurrentCenterModel);
	InserModelEntity(myCenterEntity);
}

void EntityManager::ChangeCenterModel(int index)
{
	if (myCurrentCenterModel != index)
	{
		switch (index)
		{
		case Bunny:
			myCenterModelId = "bunny";
			ReplaceCenterModel(myCenterModelId);
			myCurrentCenterModel = (MODEL)index;
			break;
		case TeaPot:
			myCenterModelId = "teapot";
			ReplaceCenterModel(myCenterModelId);
			myCurrentCenterModel = (MODEL)index;
			break;
		case Dragon:
			myCenterModelId = "dragon";
			ReplaceCenterModel(myCenterModelId);
			myCurrentCenterModel = (MODEL)index;
			break;
		default:
			break;
		}
	}
}

void EntityManager::ChangeColorMaterial(Event::UIEvtType eventype, vec3& newColor)
{
	GraphicsComponent* graphicsComponent
		= dynamic_cast<GraphicsComponent*>(myCenterEntity->GetComponent("GraphicsComponent"));

	Material* material = graphicsComponent->GetMaterial();
	switch (eventype.type)
	{
	case Event::MaterialAmbient:
		material->SetAmbient(newColor);
		break;
	case Event::MaterialDiffuse:
		material->SetDiffuse(newColor);
		break;
	case Event::MaterialSpecular:
		material->SetSpecular(newColor);
		break;
	default:
		break;
	}
}

void EntityManager::ChangeGoochParameter(Event::UIEvtType eventype, vec3 & newColor)
{
	GraphicsComponent* graphicsComponent
		= dynamic_cast<GraphicsComponent*>(myCenterEntity->GetComponent("GraphicsComponent"));

	Material* material = graphicsComponent->GetMaterial();
	switch (eventype.type)
	{
	case Event::GoochColdColor:
		material->SetColdColor(newColor);
		break;
	case Event::GoochWarmColor:
		material->SetWarmColor(newColor);
		break;
	default:
		break;
	}
}

void EntityManager::ChangeGoochParameter(Event::UIEvtType eventype, float value)
{
	GraphicsComponent* graphicsComponent
		= dynamic_cast<GraphicsComponent*>(myCenterEntity->GetComponent("GraphicsComponent"));

	Material* material = graphicsComponent->GetMaterial();
	switch (eventype.type)
	{
	case Event::GoochAlpha:
		material->SetAlpha(value);
		break;
	case Event::GoochBeta:
		material->SetBeta(value);
		break;
	default:
		break;
	}
}

void EntityManager::ChangeCelBrightLevelParameter(Event::UIEvtType eventype, float value)
{
	GraphicsComponent* graphicsComponent
		= dynamic_cast<GraphicsComponent*>(myCenterEntity->GetComponent("GraphicsComponent"));

	Material* material = graphicsComponent->GetMaterial();
	switch (eventype.type)
	{
	case Event::CelBrightLevel:
		material->SetBrighLevel(value);
		break;
	default:
		break;
	}
}

void EntityManager::onNotify(Event & event)
{
	switch (event.type)
	{
		case Event::UI:
			switch (event.ui.type)
			{
			case Event::ChangeModel:
				ChangeCenterModel(event.ui.value.modelIndex);
				break;
			case Event::MaterialAmbient:
			    {
				    vec3 color(event.ui.ambientColorR, 
						event.ui.ambientColorG, event.ui.ambientColorB);
					ChangeColorMaterial(event.ui, color);
			    }
				break;
			case Event::MaterialDiffuse:
			    {
				    vec3 color(event.ui.diffuseColorR,
					    event.ui.diffuseColorG, event.ui.diffuseColorB);
					ChangeColorMaterial(event.ui, color);
			    }
				break;
			case Event::MaterialSpecular:
			    { 
				   vec3 color(event.ui.specularColorR,
					  event.ui.specularColorG, event.ui.specularColorB);
					ChangeColorMaterial(event.ui, color);
			    }
				break;
			case Event::GoochColdColor:
				{
					vec3 color(event.ui.goochColdColorR,
						event.ui.goochColdColorG, event.ui.goochColdColorB);
					ChangeGoochParameter(event.ui, color);
				}
				break;
			case Event::GoochWarmColor:
				{
					vec3 color(event.ui.goochWarmColorR,
						event.ui.goochWarmColorG, event.ui.goochWarmColorB);
					ChangeGoochParameter(event.ui, color);
				}
				break;
			case Event::GoochAlpha:
				{
					float alpha = event.ui.goochAlpha;
					ChangeGoochParameter(event.ui, alpha);
				}
				break;
			case Event::GoochBeta:
				{
					float beta = event.ui.goochBeta;
					ChangeGoochParameter(event.ui, beta);
				}
				break;
			case Event::CelBrightLevel:
			   {
				  float level = event.ui.brightLevel;
				  ChangeCelBrightLevelParameter(event.ui, level);
			   }
				break;
			default:
				break;
			}
			break;
		default:
			break;
	}
}
