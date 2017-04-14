#pragma once
#include <glm\glm.hpp>
#include "TexturedModel.h"

using namespace glm;
class BoundingVolumen
{
public:

	virtual bool RayInstersection(vec3 start, vec3 rayDirection) = 0;
	
	virtual TexturedModel* GetModel() { return Model; };
	virtual void SetModel(TexturedModel* model) { Model = model; };

private:
	TexturedModel* Model;
};

