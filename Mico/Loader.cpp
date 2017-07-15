#include "Loader.h"

Loader* Loader::instance(0);


Loader::Loader():myModelFilePath("../Resources/Models/")
{
	
}

Loader * Loader::GetInstance()
{
	if (!instance)
	{
		instance = new Loader();
	}
	return instance;
}

Loader::~Loader()
{
}

RawModel* Loader::LoadToVAO(const vector<vec3>& positions, const vector<vec2>& textCoodinates, const vector<vec3>& normals, const vector<int>& indexes)
{
	int vao = CreateVAO();
	StoreDataInAttributeList(0, 3, positions);
	StoreDataInAttributeList(1, 2, textCoodinates);
	if (!normals.empty())
	{
		StoreDataInAttributeList(2, 3, normals);
	}
	
	BindIndicesBuffer(indexes);
	UnbindVAO();


	return  new RawModel(vao, (int)indexes.size());
}

TexturedModel* Loader::LoadToFromFile(const char * fileName)
{
	vector<TexturedModel*> models;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName,
		aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene)
	{
		printf("Mesh %s load failed\n", fileName);
	}
	this->ProcessNode(scene->mRootNode, scene, models);
	
	if (!models.empty())
	{
		return models[0];
	}
	else
	{
		return nullptr;
	}

	
}

GLuint Loader::ConstructNormalsVectorsVBO(vector<vec3>& vertices, vector<int>& indices, int& size)
{

	GLuint normal_vao = 0;
	GLuint normals_vbo = 0;
	vector<vec3> myNormals;
	for (int i = 0; i < indices.size(); i+=3)
	{
		vec3 vertex1 = vertices[indices[i]];
		vec3 vertex2 = vertices[indices[i + 1]];
		vec3 vertex3 = vertices[indices[i + 2]];

		vec3 v1 = vertex2 - vertex1;
		vec3 v2 = vertex3 - vertex1;
		vec3 normal = cross(v1, v2);

		vec3 centroid ((vertex1.x + vertex2.x + vertex3.x) / 3
			, (vertex1.y + vertex2.y + vertex3.y) / 3
			, (vertex3.z + vertex3.z + vertex3.z) / 3);
		
		myNormals.push_back(centroid);
		myNormals.push_back(centroid + normal * 6.0f);
	}

	glGenVertexArrays(1, &normal_vao);
	glGenBuffers(1, &normals_vbo);
	glBindVertexArray(normal_vao);
	glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
	glBindVertexArray(normals_vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(myNormals[0]) * myNormals.size(),
		&myNormals[0],
		GL_STATIC_DRAW
	);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	size = myNormals.size();
	return normal_vao;
}

const string & Loader::modelFilePath()
{
	return myModelFilePath;
}

void Loader::setModelFilePath(const string & modelFilePath)
{
	myModelFilePath = modelFilePath;
}



int Loader::CreateVAO()
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	vaos.push_back(vao);
	glBindVertexArray(vao);

	return vao;
}

void Loader::StoreDataInAttributeList(int attribute, int coordinateSize, const vector<vec3>& data)
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	vbos.push_back(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(vec3),
		&data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, coordinateSize, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::StoreDataInAttributeList(int attribute, int coordinateSize, const vector<vec2>&  data)
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	vbos.push_back(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(vec2),
		&data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, coordinateSize, GL_FLOAT, GL_FALSE, sizeof(vec2), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::BindIndicesBuffer(const vector<int>& indexes)
{
	GLuint ebo;
	glGenBuffers(1, &ebo);
	vbos.push_back(ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(int),
		&indexes[0], GL_STATIC_DRAW);
}

void Loader::UnbindVAO()
{
	glBindVertexArray(0);
}

void Loader::ProcessNode(aiNode* node, const aiScene* scene, vector<TexturedModel*>& models)
{
	// Process each mesh located at the current node
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		// The node object only contains indices to index the actual objects in the scene. 
		// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->ProcessMesh(mesh, scene, models);
	}
	// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene, models);
	}

}

void Loader::ProcessMesh(aiMesh* mesh, const aiScene* scene, vector<TexturedModel*>& models)
{
	// Data to fill
	vector<vec3> vertices;
	vector<vec2> uv;
	vector<vec3> normals;
	vector<int> indices;
	vector<Texture*> textures;

	// Walk through each of the mesh's vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{

		vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
					 // Positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;

		vertices.push_back(vector);

		vec3 normal;
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;

		normals.push_back(normal);

		// Texture Coordinates
		vec2 vec;
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{

			// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;

		}
		else {
			vec.x = 0.0f;
			vec.y = 0.0f;
		}


		uv.push_back(vec);
	}
	// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// Retrieve all indices of the face and store them in the indices vector
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// Process materials

	vec4 diffuseColor;
	vec4 specularColor;
	vec4 ambientColor;
	float shininess = 0.0;
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		aiString Path;

		aiColor4D diffuse(0.f, 0.f, 0.f, 1.0f);


		//aiColor3D color(0.f, 0.f, 0.f);
		//material->Get(AI_MATKEY_COLOR_DIFFUSE, color);

		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		{
			diffuseColor.r = diffuse.r;
			diffuseColor.g = diffuse.g;
			diffuseColor.b = diffuse.b;
			diffuseColor.a = diffuse.a;
		}

		aiColor4D specular;
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular))
		{
			specularColor.r = specular.r;
			specularColor.g = specular.g;
			specularColor.b = specular.b;
			specularColor.a = specular.a;
		}

		aiColor4D ambient;
		if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient))
		{
			ambientColor.r = ambient.r;
			ambientColor.g = ambient.g;
			ambientColor.b = ambient.b;
			ambientColor.a = ambient.a;
		}


		unsigned int max;
		aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &shininess, &max);


		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &Path) == AI_SUCCESS)
		{
			std::string path(Path.data);
			Texture* texture = new Texture(GL_TEXTURE_2D, "../Resources/Textures/" + path);
			texture->type = "diffuse";
			textures.push_back(texture);

		}

		if (material->GetTexture(aiTextureType_SPECULAR, 0, &Path) == AI_SUCCESS)
		{
			std::string path(Path.data);
			Texture* texture = new Texture(GL_TEXTURE_2D, "../Resources/Textures/" + path);
			texture->type = "specular";
			textures.push_back(texture);

		}
	}

	RawModel* rawModel = LoadToVAO(vertices, uv, normals, indices);
	int normalsVectorSize = 0;
	int normalsVBO = ConstructNormalsVectorsVBO(vertices, indices,normalsVectorSize);
	rawModel->setNormalsVBO(normalsVBO);
	rawModel->setNormalsCount(normalsVectorSize);
	
	TexturedModel* model = new TexturedModel(rawModel);
	/*model->SpecularColor = specularColor;
	model->DiffuseColor = diffuseColor;
	model->AmbientColor = ambientColor;
	model->Shininess = shininess;*/

	models.push_back(model);
}



void Loader::CleanUp()
{
	if(vaos.size())
		glDeleteVertexArrays((int)vaos.size(), &vaos[0]);
	
	if(vbos.size())
		glDeleteVertexArrays((int)vbos.size(),&vbos[0]);
	
	if(textures.size())
		glDeleteTextures((int)textures.size(), &textures[0]);
}