#pragma once
#include "TexturedModel.h"
#include "GraphicsComponent.h"
#include "ShaderProgram.h"
#include "Entity.h"

#define TERRAIN_SIZE 40
#define MAX_HEIGH 40


class Terrain : public Entity
{
public:

	Terrain(float x, float z);
	Terrain(float x, float z, char* heightMap);
	~Terrain();
	
	const float GetWidthX();
	const float GetWidthZ();

	void BindTextures(GraphicsComponent* graphicsCompnent);
	void Draw(ShaderProgram& shaderProgram);
	

private:

	float X;
	float Z;
	char* HeightMap;
	RawModel* GenerateTerrain();
};

