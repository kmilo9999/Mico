#include "ModelTexture.h"



ModelTexture::ModelTexture(int id):id(id)
{

}

ModelTexture::~ModelTexture()
{
}

GLuint ModelTexture::GetId()
{
	return id;
}

void ModelTexture::SetId(GLuint nid)
{
	id = nid;
}
