#pragma once
#include "RawModel.h"
#include <glm\glm.hpp>
#include <vector>
#include <map>
#include "TexturedModel.h"
#include <glad/glad.h>
#include <FreeImage\FreeImage.h>
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace glm;
using namespace std;


using TIndices = std::vector<int>;
using TFace = std::array<int, 3>;
using TFaces = std::vector<TFace>;
using TVertex = std::array<float, 3>;
using TVertices = std::vector<TVertex>;

class Loader
{

	struct Edge
	{
		Edge(uint _a, uint _b)
		{
			assert(_a != _b);

			if (_a < _b)
			{
				a = _a;
				b = _b;
			}
			else
			{
				a = _b;
				b = _a;
			}
		}

		void Print()
		{
			printf("Edge %d %d\n", a, b);
		}

		uint a;
		uint b;
	};


	struct Face
	{
		uint Indices[3];
		

		uint GetOppositeIndex(const Edge& e) const
		{
			for (uint i = 0; i < (sizeof(Indices) / sizeof(Indices[0])); i++) {
				uint Index = Indices[i];

				if (Index != e.a && Index != e.b) {
					return Index;
				}
			}

			assert(0);

			return 0;
		}
	};

	struct Neighbors
	{
		uint n1 ;
		uint n2 ;

		Neighbors();
        void AddNeigbor(uint n);
        uint GetOther(uint me) const;
	};

	struct CompareVectors
	{
		bool operator()(const aiVector3D& a, const aiVector3D& b) const
		{
			if (a.x < b.x) {
				return true;
			}
			else if (a.x == b.x) {
				if (a.y < b.y) {
					return true;
				}
				else if (a.y == b.y) {
					if (a.z < b.z) {
						return true;
					}
				}
			}

			return false;
		}
	};

	struct CompareEdges
	{
		bool operator()(const Edge& Edge1, const Edge& Edge2) const
		{
			if (Edge1.a < Edge2.a) {
				return true;
			}
			else if (Edge1.a == Edge2.a) {
				return (Edge1.b < Edge2.b);
			}
			else {
				return false;
			}
		}
	};


public:
	static Loader* GetInstance();
	~Loader();
	RawModel* LoadToVAO(const vector<vec3>& positions,
		const vector<vec2>& textCoodinates, const vector<vec3>& normals, const vector<int>& indexes);
	
	
	TexturedModel* LoadToFromFile(const char* fileName, GLenum mode);
	GLuint ConstructNormalsVectorsVBO(vector<vec3>& vertices,vector<int>& indices,int& size);

	void FindAdjacencies1(const aiMesh* paiMesh, vector<int>& Indices);
	void GenerateAdjacencies(const aiMesh* paiMesh, vector<int>& Indices);

	const string& modelFilePath();
	void setModelFilePath(const string& modelFilePath);


private:

	Loader();

	vector<GLuint> vaos;
	vector<GLuint> vbos;
	vector<GLuint> textures;

	int CreateVAO();

	void StoreDataInAttributeList(int attribute, int coordinateSize, const vector<vec3>&  data);
	void StoreDataInAttributeList(int attribute, int coordinateSize, const vector<vec2>&  data);
	void BindIndicesBuffer(const vector<int>& indexes);

	void ProcessNode(aiNode* node, const aiScene* scene, vector<TexturedModel*>& models, GLenum mode);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, vector<TexturedModel*>& models, GLenum mode);

	void GenerateTFaces(const aiMesh* paiMesh, TFaces& Indices);
	
	void CleanUp();

	void UnbindVAO();
	static Loader* instance;

	string myModelFilePath;
	std::map<Edge, Neighbors, CompareEdges> m_indexMap;
	std::map<aiVector3D, uint, CompareVectors> m_posMap;
	std::vector<Face> m_uniqueFaces;
};

