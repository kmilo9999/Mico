#include "ShadowObject.h"

ShadowObject::ShadowObject()
{
}


ShadowObject::~ShadowObject()
{
	delete myShadowFbo;
}

void ShadowObject::setShadowProjectionMatrix(mat4 const & shadowProjection)
{
	myShadowProjectionMatrix = shadowProjection;
}

const mat4 & ShadowObject::shadowProjectionMatrix()
{
	return myShadowProjectionMatrix;
}

void ShadowObject::setShadowFbo(Fbo * fbo)
{
	myShadowFbo = fbo;
}

Fbo * ShadowObject::shadowFbo()
{
	return myShadowFbo;
}
