#pragma once
#include <glm\glm.hpp>
#include "TexturedModel.h"
#include "SelectableBoundingVolumen.h"
#include "Entity.h"

using namespace glm;
class GlobalLight:public Entity
{
public:
	GlobalLight(vec3 color = vec3(0.4f , 0.2f , 0.0f), double ambient = 1.0f);
	~GlobalLight();


	vec3 GetColor();
	void setColor(vec3 nColor);

	vec3 GetAmbient();
	void SetAmbient(vec3 nScale);

	vec3 GetSpecular();
	void SetSpecular(vec3 nspecular);

	vec3 GetDiffuse();
	void SetDiffuse(vec3 nDiffuse);

	float GetIntensity();
	void SetIntensity(float intensity);


private:
	
	vec3 color;

	vec3 ambient;
	vec3 specular;
	vec3 diffuse;

	double intensity;

};

