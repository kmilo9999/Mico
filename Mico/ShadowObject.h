#pragma once
#include <glm\glm.hpp>
#include "FBO.h"

using namespace glm;

class ShadowObject
{

public:
	ShadowObject();
	~ShadowObject();


	void setShadowProjectionMatrix(mat4 const& shadowProjection);
	const mat4& shadowProjectionMatrix();

	void setShadowFbo(Fbo*);
	Fbo* shadowFbo();

private:
	mat4 myShadowProjectionMatrix;
	Fbo* myShadowFbo;
};

