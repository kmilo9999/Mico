#pragma once
#include "RawModel.h"
#include <glm\glm.hpp>
#include <vector>
#include "TexturedModel.h"
#include <glad/glad.h>
#include <FreeImage\FreeImage.h>
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace glm;
using namespace std;

class Loader
{

public:
	static Loader* GetInstance();
	~Loader();
	RawModel* LoadToVAO(const vector<vec3>& positions,
		const vector<vec2>& textCoodinates, const vector<vec3>& normals, const vector<int>& indexes);
	
	
	vector<TexturedModel*> LoadToFromFile(char* fileName);

private:

	vector<GLuint> vaos;
	vector<GLuint> vbos;
	vector<GLuint> textures;

	int CreateVAO();

	void StoreDataInAttributeList(int attribute, int coordinateSize, vector<vec3>  data);
	void StoreDataInAttributeList(int attribute, int coordinateSize, vector<vec2>  data);
	void BindIndicesBuffer(vector<int> indexes);

	void ProcessNode(aiNode* node, const aiScene* scene, vector<TexturedModel*>& models);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, vector<TexturedModel*>& models);
	
	void CleanUp();

	void UnbindVAO();
	static Loader* instance;
};

