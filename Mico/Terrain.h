#pragma once
#include "TexturedModel.h"
#include "GraphicsComponent.h"
#include "ShaderProgram.h"
#include "Entity.h"

#define TERRAIN_SIZE 60
#define MAX_HEIGHT 40

class FIBITMAP;

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

	FIBITMAP* GenerateHeightMap(const char* fileName);
	float GetHeight(unsigned int x, unsigned int y, FIBITMAP* img);
	vec3 calculateNormal(int x, int y, FIBITMAP* img);

private:

	float X;
	float Z;
	char* HeightMap;
	RawModel* GenerateTerrain();
};

