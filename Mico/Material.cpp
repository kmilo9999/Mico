#include "Material.h"

Material::Material(vec3 ambient, vec3 diffuse, vec3 specular, float shinines):
	ambient(ambient),diffuse(diffuse),specular(specular),shinines(shinines)
{
}


Material::~Material()
{
}

vec3 Material::GetAmbient()
{
	return ambient;
}

void Material::SetAmbient(vec3 n_ambient)
{
	ambient = n_ambient;
}

vec3 Material::GetDiffuse()
{
	return diffuse;
}

void Material::SetDiffuse(vec3 n_diffuse)
{
	diffuse = n_diffuse;
}

vec3 Material::GetSpecular()
{
	return specular;
}

void Material::SetSpecular(vec3 n_specular)
{
	specular = n_specular;
}


float Material::GetShinines()
{
	return shinines;
}

void Material::SetShinines(float n_shinines)
{
	shinines = n_shinines;
}

void Material::AddTexture(Texture * texture)
{
	textures.push_back(texture);
}

std::vector<Texture*> Material::GetTextures()
{
	return textures;
}

void Material::SetWarmColor(vec3 color)
{
	myWarmColor = color;
}

vec3 Material::GetWarmColor()
{
	return myWarmColor;
}

void Material::SetColdColor(vec3 cold)
{
	myColdColor = cold;
}

vec3 Material::GetColdColor()
{
	return myColdColor;
}

void Material::SetAlpha(float alpha)
{
	myAlpha = alpha;
}

float Material::GetAlpha()
{
	return myAlpha;
}

void Material::SetBeta(float beta)
{
	myBeta = beta;
}

float Material::GetBeta()
{
	return myBeta;
}

void Material::SetBrighLevel(float value)
{
	myBrighLevel = value;
}

float Material::GetBrighLevel()
{
	return myBrighLevel;
}
