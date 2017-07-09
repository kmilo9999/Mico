#include "EntityManager.h"
#include "Loader.h"
#include "SelectableBoundingVolumen.h"
#include "Event.h"
#include "TransformationComponent.h"
#include "GraphicsComponent.h"
#include <iostream>

EntityManager* EntityManager::instance(0);

vector<vec3> vertices{  vec3(0.5f,  0.5f, 0.0f),
						vec3(0.5f, -0.5f, 0.0f),
						vec3(-0.5f, -0.5f, 0.0f),
						vec3(-0.5f,  0.5f, 0.0f) };

vector<int> indexes{ 2,1,0,
					 0,3,2 };

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

TexturedModel * EntityManager::GetModel(string modelId)
{
	ModelMap::iterator it = myModels.find(modelId);
	if (it != myModels.end())
	{
		return it->second;
	}
	else
	{
		//load the model
		string filePath = Loader::GetInstance()->modelFilePath();
		string modelFilePath = filePath.append(modelId).append(".obj");
		TexturedModel* model = 
			Loader::GetInstance()->LoadToFromFile(modelFilePath.c_str());
		if (model != nullptr)
		{
			model->SetId(modelId);
			InsertModelEntities(model);
			std::cout << modelFilePath << " loaded" << std::endl;
			return model;
		}

		return nullptr;
	}
	
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
	TexturedModel* replaceModel (GetModel(modelId));
	GraphicsComponent* graphicsComponent = dynamic_cast<GraphicsComponent*>
		(myCenterEntity->GetComponent("GraphicsComponent"));
	graphicsComponent->SetTexturedModel(replaceModel);
}

void EntityManager::InitializeModels()
{
	/*vector<TexturedModel*> bunny = Loader::GetInstance()->LoadToFromFile("../Resources/Models/bunny.obj");
	vector<TexturedModel*> sphereModel = Loader::GetInstance()->LoadToFromFile("../Resources/Models/sphere.obj");
	vector<TexturedModel*> cube = Loader::GetInstance()->LoadToFromFile("../Resources/Models/cube.obj");
	vector<TexturedModel*> teapot = Loader::GetInstance()->LoadToFromFile("../Resources/Models/teapot.obj");
	vector<TexturedModel*> dragon = Loader::GetInstance()->LoadToFromFile("../Resources/Models/dragon.obj");


	TexturedModel* model = bunny[0];
	model->SetId("bunny");
	InsertModelEntities(model);
	
	model = teapot[0];
	model->SetId("teapot");
	InsertModelEntities(model);

	model = dragon[0];
	model->SetId("dragon");
	InsertModelEntities(model);

	model = sphereModel[0];
	model->SetId("sphere");
	InsertModelEntities(model);

	model = cube[0];
	model->SetId("cube");
	InsertModelEntities(model);
*/

	/*RawModel* rawModel = Loader::GetInstance()->LoadToVAO(vertices, textcoor, vector<vec3>(), indexes);
	int normalsVectorSize = 0;
	int normalsVBO = Loader::GetInstance()->ConstructNormalsVectorsVBO(vertices, indexes, normalsVectorSize);
	rawModel->setNormalsVBO(normalsVBO);
	rawModel->setNormalsCount(normalsVectorSize);
	Texture* targetTtexture2 = new Texture(GL_TEXTURE_2D, "../Resources/Textures/dragonballzsuper.jpg");
	Material* material2 = new Material(vec3(1.0f, 0.5f, 0.31f), vec3(1.0f, 0.5f, 0.31f), vec3(0.5f, 0.5f, 0.5f), 64.0f);
	targetTtexture2->SetMaterial(material2);
	vector<Texture*> myTextures = { targetTtexture2 };
	TexturedModel* myModel = new TexturedModel(rawModel, myTextures);
	
	myModel->SetId("plane");
	InsertModelEntities(myModel);*/
}

void EntityManager::Initialize()
{
	
	//Initialize center model
	//InitializeModels();
	CreateEntity("bunny", vec3(0.0f, 11.0f, 3.0f), quat(), vec3(1.0f, 1.0f, 1.0f));
	myCenterModelId = "bunny";



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

void EntityManager::CreateEntity(string modelId, vec3 postion, quat orientation, vec3 scale)
{
	myCenterEntity = new Entity();
	TransformationComponent* transformation = new TransformationComponent(postion, orientation, scale);
	myCenterEntity->AddComponent(transformation);
	
	Texture* targetTtexture = new Texture(GL_TEXTURE_2D, "../Resources/Textures/texture_sample.jpg");
	Material* material = new Material(vec3(1.0f, 0.5f, 0.31f), vec3(1.0f, 0.5f, 0.31f), vec3(0.5f, 0.5f, 0.5f), 64.0f);
	material->AddTexture(targetTtexture);
	GraphicsComponent* graphics = new GraphicsComponent(GetModel(modelId), material);
	myCenterEntity->AddComponent(graphics);

    SelectableBoundingVolumen* boundingVolumen = new BoundingSphere(0.7f);
	boundingVolumen->SetModel(GetModel("sphere"));
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
			default:
				break;
			}
			break;
		default:
			break;
	}
}
