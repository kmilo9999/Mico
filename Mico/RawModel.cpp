#include "RawModel.h"



RawModel::RawModel(int vao, int indexCount):vao(vao),myIndexCount(indexCount)
{
}

RawModel::~RawModel()
{
}

int RawModel::getVao()
{
	return this->vao;
}

int RawModel::getVao() const
{
	return this->vao;
}

void RawModel::setVao(int vao)
{
	this->vao = vao;
}

int RawModel::getIndexCount()
{
	return myIndexCount;
}

int RawModel::getIndexCount() const
{
	return myIndexCount;
}

void RawModel::setVertexCount(int vertexcount)
{
	this->myIndexCount = myIndexCount;
}

int RawModel::getNormalsVBO()
{
	return normals_vbo;
}

void RawModel::setNormalsVBO(int normals_vbo)
{
	this->normals_vbo = normals_vbo;
}

int RawModel::getNormalsCount()
{
	return normalsCount;
}

void RawModel::setNormalsCount(int size)
{
	normalsCount = size;
}
