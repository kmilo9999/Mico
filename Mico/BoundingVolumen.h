#pragma once
#include <glm\glm.hpp>
#include "TexturedModel.h"
#include "Component.h"

using namespace glm;
class Entity;
class BoundingVolumen: public Component
{
	
public:

	virtual bool RayInstersection(vec3 start, vec3 rayDirection) = 0;
	
	virtual TexturedModel* GetModel() { return Model; };
	virtual void SetModel(TexturedModel* model) { Model = model; };

	virtual vec3 GetPosition();
	virtual void SetPotision(vec3 pos);


	virtual void Update() {};

	virtual void Initialize(Entity* owner) { myOwner = owner; };
	virtual void ShutDown() {};

protected:
	vec3 Position;
	

private:
	TexturedModel* Model;
	
};

