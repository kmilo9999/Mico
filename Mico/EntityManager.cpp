#include "EntityManager.h"
#include "Loader.h"
#include "SelectableBoundingVolumen.h"

EntityManager* EntityManager::instance(0);

EntityManager::EntityManager()
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
	return entities;
}

EntityManager::~EntityManager()
{
	for (EntityMap::iterator itr = entities.begin();
		itr != entities.end(); itr++)
	{
		vector<Entity*> entity = itr->second;
		for (size_t i = 0; i < entity.size(); ++i)
		{
			delete entity[i];
		}
		delete[] itr->first;
		entities.erase(itr);
	}

	if (instance)
	{
		delete instance;
	}
}

void EntityManager::InitializeModels()
{
	vector<TexturedModel*> bunny = Loader::GetInstance()->LoadToFromFile("../Resources/Models/bunny.obj");
	vector<TexturedModel*> sphereModel = Loader::GetInstance()->LoadToFromFile("../Resources/Models/sphere.obj");
	vector<TexturedModel*> cube = Loader::GetInstance()->LoadToFromFile("../Resources/Models/cube.obj");


	TexturedModel* model = bunny[0];
	model->SetId("bunny");
	Texture* targetTtexture = new Texture(GL_TEXTURE_2D, "../Resources/Textures/texture_sample.jpg");
	Material* material = new Material(vec3(1.0f, 0.5f, 0.31f), vec3(1.0f, 0.5f, 0.31f), vec3(0.5f, 0.5f, 0.5f), 64.0f);
	targetTtexture->SetMaterial(material);
	model->Addtexture(targetTtexture);
	
	

	InsertModelEntities(model);
	
	model = sphereModel[0];
	model->SetId("sphere");
	InsertModelEntities(model);

	model = cube[0];
	model->SetId("cube");
	InsertModelEntities(model);
}

void EntityManager::Initialize()
{
	
	//Initialize center model
	InitializeModels();
	Entity* target = new Entity();
	target->SetPosition(vec3(0.0f, 10.0f, 0.0f));
	target->SetScale(vec3(1.0f, 1.0f, 1.0f));

	SelectableBoundingVolumen* sphere = new BoundingSphere(target->GetPosition(),0.7f);
	sphere->SetModel(GetModel("sphere"));

	target->SetBoundingVolumen(sphere);
	
	InserModelEntity("bunny", target);

}


TexturedModel* EntityManager::GetModel(string model)
{
	map<TexturedModel*, vector<Entity*>, TexturedModelCompare> ::iterator it = entities.begin();

	for (;it != entities.end(); ++it)
	{
		TexturedModel* texturedModel = it->first;
		if (texturedModel->GetId() == model)
		{
			return texturedModel;
		}
	}

	return nullptr;
}

void EntityManager::InsertModelEntities(TexturedModel * model)
{
	vector<Entity*> entitiesVector;
	entities.insert(std::pair <TexturedModel*, vector<Entity*>>(model, entitiesVector));
}

void EntityManager::InserModelEntity(string model, Entity * entity)
{
	
	EntityMap::iterator it = entities.begin();

		for (;it != entities.end(); ++it)
		{
			TexturedModel* texturedModel = it->first;
			if (texturedModel->GetId() == model)
			{
				it->second.push_back(entity);
			}
		}
	

}
