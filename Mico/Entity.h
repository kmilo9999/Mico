#pragma once
#include "TexturedModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "BoundingSphere.h"

using namespace glm;
class Entity
{
public:
	Entity(vec3 postion=vec3(),quat orientation=quat(), vec3 scale = vec3() );
	~Entity();


	virtual vec3 GetPosition();
	virtual void SetPosition(vec3 position);

	virtual vec3 GetScale();
	virtual void SetScale(vec3 scale);

	virtual quat GetOrientation();
	virtual void SetOrientatio(quat orientation);

	void SetBoundingVolumen(BoundingVolumen* volumen);
	BoundingVolumen* GetBoundingVolumen();


protected:
	vec3 Position;
	quat Orientation;
	vec3 Scale;
	BoundingVolumen* boundingVolumen;
};

