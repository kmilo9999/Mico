#pragma once
#include <glad\glad.h>
#include <FreeImage\FreeImage.h>
#include <string>

class Texture
{

public:

	Texture(GLenum TextureTarget, const std::string& filename);
	~Texture();


	void Bind(GLenum TextureUnit);
	GLuint GetTextureId();

	std::string type;

private:

	int LoadTexture();
	GLuint texture_id;
	GLenum m_textureTarget;
	const char* m_fileName;
};

