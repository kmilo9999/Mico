#pragma once
#include <glm\glm.hpp>

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

private:
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shinines;

};

