#pragma once
class RawModel
{
public:
	RawModel(int vao, int vertexCount);
	
	~RawModel();

	int getVao();
	int getVao() const;

	void setVao(int vao);

	int getVertexCount();
	int getVertexCount() const;
	void setVertexCount(int vertexcount);

	int getNormalsVAO();
	void setNormalsVBO(int normals_vbo);

	int getNormalsCount();
	void setNormalsCount(int size);

protected:
	int vao;
	int normals_vbo;
	int vertexCount;
	int normalsCount;
	

};

