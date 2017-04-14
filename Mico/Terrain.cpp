#include "Terrain.h"
#include <cassert>
#include "Loader.h"

#define MAX_PIXEL_COLOR 256 * 256 * 256

Terrain::Terrain(float x, float z):X(x),Z(z)
{
	Texture* texture = new Texture(GL_TEXTURE_2D, "../Resources/Textures/terrain.jpg");
	vector<Texture*> textures;
	textures.push_back(texture);
	Model = new TexturedModel(GenerateTerrain(), textures);
}


Terrain::Terrain(float x, float z, char* heightMap)
{

}

Terrain::~Terrain()
{

}

const float Terrain::GetWidthX()
{
	return this->X;
}

const float Terrain::GetWidthZ()
{
	return this->Z;
}

TexturedModel* Terrain::GetModel()
{
	return this->Model;
}

RawModel* Terrain::GenerateTerrain()
{
	int VERTEX_COUNT = 128;
	int count = VERTEX_COUNT * VERTEX_COUNT;
	int size3 = count * 3;
	int size2 = count * 2;

	vector< vec3 > vVertexData;
	vector< vec3> vNormalsData;
	vector< vec2 > vCoordsData;
	vector< int > indices;

	int vertexPointer = 0;

	float halfW = -((float)TERRAIN_SIZE / (float)2);
	float move = (float)TERRAIN_SIZE / (float)(VERTEX_COUNT - 1);

	for (float i = 0; i< VERTEX_COUNT; i++) {

		float halfL = -((float)TERRAIN_SIZE / (float)2);
		for (float j = 0; j < VERTEX_COUNT; j++) {


			float vx = ((float)halfL / ((float)VERTEX_COUNT - 1)) * TERRAIN_SIZE;
			float vy = 0; // GetHeight(i, j, heighMap);
			float vz = ((float)halfW / ((float)VERTEX_COUNT - 1))* TERRAIN_SIZE;
			vec3 vertex(vx, vy, vz);
			vVertexData.push_back(vertex);
			halfL += move;


			float nx = 0.0f;
			float ny = 1.0f;
			float nz = 0.0f;
			vec3 normal(vx, vy, vz);
			vNormalsData.push_back(normal);

			float uvx = (float)j / ((float)VERTEX_COUNT - 1);
			float uvy = (float)i / ((float)VERTEX_COUNT - 1);
			vec2 uv(uvx, uvy);
			vCoordsData.push_back(uv);



		}
		halfW += move;
	}

	int pointer = 0;
	for (int gz = 0; gz<VERTEX_COUNT - 1; gz++) {
		for (int gx = 0; gx<VERTEX_COUNT - 1; gx++) {
			int topLeft = (gz*VERTEX_COUNT) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1)*VERTEX_COUNT) + gx;
			int bottomRight = bottomLeft + 1;
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}


	return Loader::GetInstance()->LoadToVAO(vVertexData, vCoordsData, vNormalsData, indices);
}