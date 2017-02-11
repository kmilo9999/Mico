#pragma once
class RawModel
{
public:
	RawModel(int vao, int vertexCount);
	~RawModel();

	int getVao();
	void setVao(int vao);

	int getVertexCount();
	void setVertexCount(int vertexcount);

protected:
	int vao;
	int vertexCount;

	

};

