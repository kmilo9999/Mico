#pragma once
#include "SelectableBoundingVolumen.h"

class BoundingSphere : public SelectableBoundingVolumen
{
public:
	BoundingSphere( float radius);
	~BoundingSphere();

	bool RayInstersection(vec3 start, vec3 rayDirection);

	vec3 GetCenter();
	void SetCenter(vec3 n_center);

	float GetRadius();
	void SetRadius(float n_radius);
	
	virtual void Update();

	virtual void Initialize(Entity* owner);
	virtual void ShutDown();

private:
	vec3 center;
	float radius;
};

