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