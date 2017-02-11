#pragma once
#include "TexturedModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;
class Entity
{
public:
	Entity(vec3 postion=vec3(),quat orientation=quat(), vec3 scale = vec3() );
	~Entity();


	vec3 GetPosition();
	void SetPosition(vec3 position);

	vec3 GetScale();
	void SetScale(vec3 scale);

	quat GetOrientation();
	void SetOrientatio(quat orientation);

private:
	vec3 Position;
	quat Orientation;
	vec3 Scale;
};

