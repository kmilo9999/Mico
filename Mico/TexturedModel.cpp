#include "TexturedModel.h"


TexturedModel::TexturedModel(RawModel * model, vector<Texture*>&  m_textures): model(model),textures(m_textures)
{
}

TexturedModel::~TexturedModel()
{
	delete model;
	for (size_t i = 0; i < textures.size(); ++i)
	{
		delete textures[i];
	}
}

RawModel * TexturedModel::GetModel()
{
	return this->model;
}

void TexturedModel::SetModel(RawModel * nModel)
{
	this->model = nModel;
}

void TexturedModel::BindTexture(ShaderProgram& shader)
{
	for (size_t i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		textures[i]->Bind(GL_TEXTURE0 + i);
		if (textures[i]->GetMaterial())
		{
			shader.setUniform("material.ambient", textures[i]->GetMaterial()->GetAmbient());
			shader.setUniform("material.diffuse", textures[i]->GetMaterial()->GetDiffuse());
			shader.setUniform("material.specular", textures[i]->GetMaterial()->GetSpecular());
			shader.setUniformf("material.shininess", textures[i]->GetMaterial()->GetShinines());
		}
	}
	

}

void TexturedModel::Bind()
{
	for (size_t i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		textures[i]->Bind(GL_TEXTURE0 + i);
	}


}

void TexturedModel::Draw()
{
	glBindVertexArray(model->getVao());
	glDrawElements(GL_TRIANGLES, model->getVertexCount(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void TexturedModel::UnBind()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glActiveTexture(0);
	glBindVertexArray(0);
}

void TexturedModel::Addtexture(Texture* texture)
{
	textures.push_back(texture);
}

void TexturedModel::SetId(string id)
{
	this->id = id;
}

string TexturedModel::GetId()
{
	return id;
}

unsigned int TexturedModel::NumTextures()
{
	return textures.size();
}
