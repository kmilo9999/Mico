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


	TexturedModel* GetModelByName(string model);
	TexturedModel* GetModelByNameAndMode(string model, GLenum mode );
	TexturedModel* GetModel(string model, GLenum mode);
	
	void InsertModelEntities(TexturedModel* model );

	void InserModelEntity( Entity* entity);
	void RemoveEntity( unsigned int entityId);
	void CreateEntity(string modelId, vec3 postion = vec3(), quat orientation = quat(), vec3 scale = vec3(), GLenum mode = GL_TRIANGLES);

	virtual void onNotify(Event& event);

	~EntityManager();

private:

	void ReplaceCenterModel(string modelId);

	EntityManager();
	EntityMap myEntities;
	ModelMap myModels;
	void ChangeCenterModel(int index);
	void ChangeColorMaterial(Event::UIEvtType type, vec3& newColor);
	void ChangeGoochParameter(Event::UIEvtType type, vec3& newColor);
	void ChangeGoochParameter(Event::UIEvtType type, float);
	void ChangeCelBrightLevelParameter(Event::UIEvtType type, float);
	static EntityManager* instance;
	MODEL myCurrentCenterModel;
	string myCenterModelId;
	Entity* myCenterEntity;
};

