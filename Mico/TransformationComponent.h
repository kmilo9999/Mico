#pragma once
#include "Component.h"
#include <glm\common.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;
class TransformationComponent : public Component
{
public:
	TransformationComponent(vec3 postion = vec3(), quat orientation = quat(), vec3 scale = vec3());
	~TransformationComponent();

	virtual void Update();

	vec3 GetPosition();
	virtual void SetPosition(vec3 position);

	vec3 GetScale();
	void SetScale(vec3 scale);

	quat GetOrientation();
	void SetOrientation(quat orientation);

	void Rotate(quat rotation);

	virtual void Initialize(Entity* owner);
	virtual void ShutDown();

private:
	vec3 myPosition;
	quat myOrientation;
	vec3 myOrientationAxis;
	vec3 myScale;


};

