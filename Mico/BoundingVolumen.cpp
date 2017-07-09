#include "BoundingVolumen.h"


vec3 BoundingVolumen::GetPosition()
{
	return Position;
}

void BoundingVolumen::SetPotision(vec3 pos)
{
	Position = pos;
}

