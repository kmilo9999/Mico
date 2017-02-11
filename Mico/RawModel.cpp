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

void RawModel::setVao(int vao)
{
	this->vao = vao;
}

int RawModel::getVertexCount()
{
	return vertexCount;
}

void RawModel::setVertexCount(int vertexcount)
{
	this->vertexCount = vertexCount;
}
