#include "GlobalLight.h"
#include "TransformationComponent.h"

GlobalLight::GlobalLight(vec3 color, double ambient)
	:color(color),ambient(ambient)
{

}

GlobalLight::~GlobalLight()
{

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
