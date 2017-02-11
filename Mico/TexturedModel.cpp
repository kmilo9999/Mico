#include "TexturedModel.h"




TexturedModel::TexturedModel(RawModel * model, ModelTexture texture): model(model),texture(texture)
{
}

TexturedModel::~TexturedModel()
{
}

RawModel * TexturedModel::GetModel()
{
	return this->model;
}

void TexturedModel::SetModel(RawModel * nModel)
{
	this->model = nModel;
}

ModelTexture TexturedModel::GetModelTexture()
{
	return this->texture;
}

void TexturedModel::SetModelTexture(ModelTexture texture)
{
	this->texture = texture;
}

void TexturedModel::Bind()
{

	
	glBindVertexArray(model->getVao());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.GetId());
	

}

void TexturedModel::Draw()
{
	glDrawElements(GL_TRIANGLES, model->getVertexCount(), GL_UNSIGNED_INT, 0);
}

void TexturedModel::UnBind()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glActiveTexture(0);
	glBindVertexArray(0);
}