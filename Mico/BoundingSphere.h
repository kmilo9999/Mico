#pragma once
#include "SelectableBoundingVolumen.h"

class BoundingSphere : public SelectableBoundingVolumen
{
public:
	BoundingSphere(vec3 center, float radius);
	~BoundingSphere();

	bool RayInstersection(vec3 start, vec3 rayDirection);

	vec3 GetCenter();
	void SetCenter(vec3 n_center);

	float GetRadius();
	void SetRadius(float n_radius);

private:
	vec3 center;
	float radius;
};

