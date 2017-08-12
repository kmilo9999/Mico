#include "TexturedModel.h"


TexturedModel::TexturedModel(RawModel * model): myModel(model)
{

}

TexturedModel::TexturedModel(const TexturedModel & other)
	: myModel(new RawModel(other.GetModel()->getVao(), other.GetModel()->getVertexCount()))
{
	
}

TexturedModel & TexturedModel::operator=(const TexturedModel & other)
{
	// TODO: insert return statement here
    myModel = new RawModel(other.GetModel()->getVao(), other.GetModel()->getVertexCount());
	return *this;
}

TexturedModel::~TexturedModel()
{
	delete myModel;
}

RawModel * TexturedModel::GetModel()
{
	return this->myModel;
}

RawModel * TexturedModel::GetModel() const
{
	return this->myModel;
}

void TexturedModel::SetModel(RawModel * nModel)
{
	this->myModel = nModel;
}

void TexturedModel::BindTexture(std::vector<Texture*>& textures)
{
	for (size_t i = 0; i < textures.size(); ++i)
	{
		textures[i]->Bind( i);
	}
}

void TexturedModel::BindTexture(int textureUnit, int textureId)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

void TexturedModel::Draw()
{
	glBindVertexArray(myModel->getVao());
	glDrawElements(GL_TRIANGLES, myModel->getVertexCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void TexturedModel::DrawNormals()
{
	//glDisable(GL_DEPTH_TEST);
	glBindVertexArray(myModel->getNormalsVBO());
	glDrawArrays(GL_LINES, 0, myModel->getNormalsCount());
	glBindVertexArray(0);
	//glEnable(GL_DEPTH_TEST);
}

void TexturedModel::UnBind()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glActiveTexture(0);
	glBindVertexArray(0);
}

void TexturedModel::SetId(string id)
{
	this->id = id;
}

string TexturedModel::GetId()
{
	return id;
}


