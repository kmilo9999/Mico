#include "Terrain.h"
#include <cassert>
#include "Loader.h"
#include <iostream>

#define MAX_PIXEL_COLOR 256 


Terrain::Terrain(float x, float z):X(x),Z(z)
{
	
	EntityId = 1000;
	Texture* texture = new Texture(GL_TEXTURE_2D, "../Resources/Textures/texture_sample.jpg");
	//Texture* texture = new Texture(GL_TEXTURE_2D, "../Resources/Textures/hatching/hatching_6.jpg");
	
	RawModel* terrainModel = GenerateTerrain();
	TexturedModel* model = new TexturedModel(GL_TRIANGLES, terrainModel);

	// The terrain is not doing any light calcultations. I just put the  material becasue 
	// needed to initialize the GraphicsComponent
	Material* material = 
		new Material(vec3(0.1f, 0.1f, 0.1f), vec3(0.3, 0.2, 0.1),
			vec3(1.0f), 32.0f);

	material->AddTexture(texture);

	TransformationComponent* transformation = 
		new TransformationComponent(vec3(X, 0, Z),quat(),vec3(1.0f, 1.0f, 1.0f));
	AddComponent(transformation);

	GraphicsComponent* myGraphicsComponent = new GraphicsComponent(model, material);
	AddComponent(myGraphicsComponent);

	Initialize();

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

void Terrain::BindTextures(GraphicsComponent* graphicsCompnent)
{
	graphicsCompnent->bindModelTextures();
}

void Terrain::Draw(ShaderProgram & shaderProgram)
{
	GraphicsComponent* myGraphicsComponent
		= dynamic_cast<GraphicsComponent*>(GetComponent("GraphicsComponent"));
	myGraphicsComponent->Draw(shaderProgram);
}

FIBITMAP* Terrain::GenerateHeightMap(const char * fileName)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	// check the file signature and deduce its format
	// (the second argument is currently not used by FreeImage)
	fif = FreeImage_GetFileType(fileName, 0);
	if (fif == FIF_UNKNOWN) {
		// no signature ?
		// try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(fileName);
	}
	// check that the plugin has reading capabilities ...
	if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		// ok, let's load the file
		FIBITMAP *dib = FreeImage_Load(fif, fileName);
		// unless a bad file format, we are done !
		return dib;
	}
	return NULL;

}

float Terrain::GetHeight(unsigned int x, unsigned int y, FIBITMAP* bitmap)
{
	RGBQUAD color; 
	if(FreeImage_GetPixelColor(bitmap, x, y, &color))
	{
		float c = MAX_HEIGHT * ((color.rgbRed / 255.0f) - 0.5f);
		return c;
	}
	return 0;
}

vec3  Terrain::calculateNormal(int x, int y, FIBITMAP * img)
{
	float heightL = GetHeight(x - 1, y, img);
	float heightR = GetHeight(x + 1, y, img);
	float heightD = GetHeight(x , y - 1, img);
	float heightU = GetHeight(x , y + 1, img);
	vec3 normal(heightL - heightR, 2.0f, heightD - heightU);
	normal = normalize(normal);
	return normal;
}


RawModel* Terrain::GenerateTerrain()
{
	FIBITMAP* heighMap = GenerateHeightMap("../Resources/Textures/heightmap.png");
	unsigned int width = 0;
	unsigned int height = 0;
	if (heighMap)
	{
		width = FreeImage_GetWidth(heighMap);
		height = FreeImage_GetHeight(heighMap);
		//GetHeight(x, y, bitmap);
		
	}


	int VERTEX_COUNT = 256;
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
			//float vy = !heighMap ? GetHeight(j, i, heighMap):0;
			float vy = 0;
			
			float vz = ((float)halfW / ((float)VERTEX_COUNT - 1))* TERRAIN_SIZE;
			vec3 vertex(vx, vy, vz);
			vVertexData.push_back(vertex);
			halfL += move;

			//vec3 normal = calculateNormal(j, i, heighMap);
			vec3 normal = vec3(0.0f, 1.0f, 0.0f);
			/*float nx = normal.x;
			float ny = normal.y;
			float nz = normal.z;
			vec3 normal(vx, vy, vz);*/
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

	if (heighMap)
	{
		FreeImage_Unload(heighMap);
	}

	return Loader::GetInstance()->LoadToVAO(vVertexData, vCoordsData, vNormalsData, indices);
}