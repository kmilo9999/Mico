#pragma once
#include "TexturedModel.h"

#define TERRAIN_SIZE 40
#define MAX_HEIGH 40


class Terrain
{
public:

	Terrain(float x, float z);
	Terrain(float x, float z, char* heightMap);
	~Terrain();
	
	const float GetWidthX();
	const float GetWidthZ();
	TexturedModel* GetModel();

private:

	float X;
	float Z;
	char* HeightMap;
	TexturedModel* Model;
	RawModel* GenerateTerrain();

};

