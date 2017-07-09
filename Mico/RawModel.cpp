#include "RawModel.h"



RawModel::RawModel(int vao, int vertexCount):vao(vao),vertexCount(vertexCount)
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

int RawModel::getVertexCount()
{
	return vertexCount;
}

int RawModel::getVertexCount() const
{
	return vertexCount;
}

void RawModel::setVertexCount(int vertexcount)
{
	this->vertexCount = vertexCount;
}

int RawModel::getNormalsVAO()
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
