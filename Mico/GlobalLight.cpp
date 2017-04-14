#include "GlobalLight.h"


GlobalLight::GlobalLight(vec3 position, vec3 color, double ambient)
	:color(color),ambient(ambient)
{
	this->Position = position;
}

GlobalLight::~GlobalLight()
{
	if (model)
	{
		delete model;
	}
}

vec3 GlobalLight::GetColor()
{
	return color;
}

void GlobalLight::setColor(vec3 nColor)
{
	color = nColor;
}

vec3 GlobalLight::GetAmbient()
{
	return ambient;
}

void GlobalLight::SetAmbient(vec3 nAmbient)
{
	this->ambient = nAmbient;
}

float GlobalLight::GetIntensity()
{
	return intensity;
}

void GlobalLight::SetIntensity(float intensity)
{
	this->intensity = intensity;
}

TexturedModel* GlobalLight::GetModel()
{
	return model;
}

void GlobalLight::SetModel(TexturedModel * _model)
{
	model = _model;
}

vec3 GlobalLight::GetSpecular()
{
	return specular;
}

void GlobalLight::SetSpecular(vec3 nspecular)
{
	this->specular = nspecular;
}

vec3 GlobalLight::GetDiffuse()
{
	return diffuse;
}
void GlobalLight::SetDiffuse(vec3 nDiffuse)
{
	this->diffuse = nDiffuse;
}

BoundingVolumen* GlobalLight::GetBoundingVolumen()
{
	return boundingVolumen;
}

void GlobalLight::SetBoundingVolumen(BoundingVolumen* volumen)
{
	boundingVolumen = volumen;
}