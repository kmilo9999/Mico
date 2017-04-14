#pragma once
#include <map>
#include "TexturedModel.h"
#include "Entity.h"



class EntityManager
{
public:
	typedef map<TexturedModel*, vector<Entity*>, TexturedModelCompare> EntityMap;
	static EntityManager* GetInstance();

	void Initialize();
	
	EntityMap & GetEntities();
	TexturedModel* GetModel(string model);
	
	void InsertModelEntities(TexturedModel* model );
	void InserModelEntity(string model, Entity* entity);

	~EntityManager();

private:
	void InitializeModels();
	EntityManager();
	EntityMap entities;
	static EntityManager* instance;
};

