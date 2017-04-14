#include "Texture.h"
#include <cassert>


Texture::Texture(GLenum TextureTarget, const std::string& filename) :
	m_textureTarget(TextureTarget), m_fileName(filename.c_str())
{
	texture_id = LoadTexture();
}

Texture::~Texture()
{
	if (material)
	{
		delete material;
	}
}

int Texture::LoadTexture()
{

	GLuint textureID;

	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	//OpenGL's image ID to map to


	GLuint gl_texID;
	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(m_fileName, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
	{
		fif = FreeImage_GetFIFFromFilename(m_fileName);
	}
	

	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
	{
		assert(false && "texture unknow file format");
	}
	

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, m_fileName);

	//if the image failed to load, return failure
	if (!dib)
	{
	    assert(false && "Image failed to load 1");
	}

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
	{
		assert(false && "Image failed to load 2");
	}
		

	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


	// Read the file, call glTexImage2D with the right parameters
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		0, GL_BGR, GL_UNSIGNED_BYTE, bits);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

	// Return the ID of the texture we just created
	return textureID;
}


void Texture::Bind(GLenum TextureUnit)
{
	glActiveTexture(GL_TEXTURE0 + TextureUnit);
	glBindTexture(m_textureTarget, texture_id);
}

GLuint Texture::GetTextureId()
{
	return texture_id;
}

void Texture::SetMaterial(Material* material)
{
	this->material = material;
}

Material* Texture::GetMaterial()
{
	return material;
}
