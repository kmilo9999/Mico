#include "BoundingSphere.h"



BoundingSphere::BoundingSphere(vec3 center, float radius): center(center),radius(radius)
{

}


BoundingSphere::~BoundingSphere()
{
}

bool BoundingSphere::RayInstersection(vec3 start, vec3 rayDirection)
{
	float b = 2 * (rayDirection.x * (start.x - center.x) + 
		rayDirection.y * (start.y - center.y) + rayDirection.z * (start.z - center.z));
	float c = start.x * start.x - 2 * start.x * center.x + center.x * center.x
		+ start.y * start.y - 2 * start.y * center.y + center.y * center.y
		+ start.z * start.z - 2 * start.z * center.z + center.z * center.z - radius * radius;

	float discr = (b * b - 4 * c);
	if (discr < 0)
	{
		selected = false;
		return false;
	}
	else
	{
		selected = true;
		return true;
	}
	
}

vec3 BoundingSphere::GetCenter()
{
	return center;
}

void BoundingSphere::SetCenter(vec3 n_center)
{
	center = n_center;
}

float BoundingSphere::GetRadius()
{
	return radius;
}

void BoundingSphere::SetRadius(float n_radius)
{
	radius = n_radius;
}
