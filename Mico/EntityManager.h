#pragma once
#include <map>
#include "TexturedModel.h"
#include "Entity.h"
#include "Observer.h"


class EntityManager: public Observer
{
	

public:
	
	enum MODEL
	{
		Bunny,
		TeaPot,
		Dragon
	};

	typedef vector<Entity*> EntityMap;
	
	typedef map<string, TexturedModel*> ModelMap;

	static EntityManager* GetInstance();

	void Initialize();
	
	EntityMap & GetEntities();
	void UpdateEntities();


	TexturedModel* GetModel(string model);
	
	void InsertModelEntities(TexturedModel* model );

	void InserModelEntity( Entity* entity);
	void RemoveEntity( unsigned int entityId);
	void CreateEntity(string modelId, vec3 postion = vec3(), quat orientation = quat(), vec3 scale = vec3());

	virtual void onNotify(Event& event);

	~EntityManager();

private:

	void ReplaceCenterModel(string modelId);

	void InitializeModels();
	EntityManager();
	EntityMap myEntities;
	ModelMap myModels;
	void ChangeCenterModel(int index);
	void ChangeColorMaterial(Event::UIEvtType type, vec3& newColor);
	static EntityManager* instance;
	MODEL myCurrentCenterModel;
	string myCenterModelId;
	Entity* myCenterEntity;
};

