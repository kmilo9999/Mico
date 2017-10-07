#pragma once
#include <glm\glm.hpp>
#include <vector>
#include "Texture.h"
#include "Component.h"
using namespace glm;

class Material
{
public:
	Material(vec3 ambient,vec3 diffuse, vec3 specular, float shinines);
	~Material();

	vec3 GetAmbient();
	void SetAmbient(vec3 n_ambient);
	
	vec3 GetDiffuse();
	void SetDiffuse(vec3 n_diffuse);

	vec3 GetSpecular();
	void SetSpecular(vec3 n_specular);


	float GetShinines();
	void SetShinines(float n_shinines);

	void AddTexture(Texture* texture);

	std::vector<Texture*>  GetTextures();

	void SetWarmColor(vec3 );
	vec3 GetWarmColor();

	void SetColdColor(vec3 );
    vec3 GetColdColor();

	void SetAlpha(float);
	float GetAlpha();
	
	void  SetBeta(float);
	float GetBeta();

	void SetBrighLevel(float);
	float GetBrighLevel();

private:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shinines;
	std::vector<Texture*>  textures;

	// Gooch parameters
	vec3 myWarmColor;
	vec3 myColdColor;
	float myAlpha;
	float myBeta;

	// Cel shading, levels of brightenes
	float myBrighLevel;

};

