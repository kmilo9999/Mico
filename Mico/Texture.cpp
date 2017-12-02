#include "Texture.h"
#include <cassert>


Texture::Texture(GLenum TextureTarget, const std::string& filename)
	: myTextureTarget(TextureTarget)
	, myFileName(filename.c_str())
	, myIs2D(true)
{
   texture_id = Load2DTexture();
}

Texture::Texture(GLenum TextureTarget, const std::vector<std::string>& paths)
	: myTextureTarget(TextureTarget)
	, myIs2D(false)
{
	texture_id = Load3DTexture(paths);
}

Texture::~Texture()
{

}

int Texture::Load2DTexture()
{

	GLuint textureID;

	int width = 0;
	int height = 0;
	//pointer to the image data
	BYTE * bits(0);

	//retrieve the image data
	//if this somehow one of these failed (they shouldn't), return failure
	if (!LoadTexture(myFileName,width,height, &bits))
	{
		assert(false && "Image failed to load 2");
	}

	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);


	// Read the file, call glTexImage2D with the right parameters
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		0, GL_RGB, GL_UNSIGNED_BYTE, bits);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Return the ID of the texture we just created
	return textureID;
}

int Texture::Load3DTexture(const std::vector<std::string>& paths)
{
	
	if (paths.size() == 0)
	{
		assert(false && "no paths to load from");
	}

	GLsizei width, height, depth = paths.size();

	std::vector<image> formatedImages(paths.size());

	// load and format each image
	for (int i = 0; i < paths.size(); ++i)
	{
		BYTE * bits(0);
		if (!LoadTexture(paths[i], width, height, &bits))
		{
			assert(false && "Image failed to load 2");
		}
		image img(width, height, bits);
		formatedImages[i] = img;
	}

	//// create empty 3D texture

	//GLuint textureID;
	//glGenTextures(1, &textureID);
	//glBindTexture(GL_TEXTURE_3D, textureID);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//// allocate memory for 3D texture
	//glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, width,
	//	height, depth, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);


	//// copy image data to each layer of 3D texture

	//for (int i = 0; i < formatedImages.size() ; ++i)
	//{
	//	glTexSubImage3D(
	//		GL_TEXTURE_3D, 0,
	//		0, 0, (GLint)i,
	//		formatedImages[i].imageWidth, formatedImages[i].imageHeight, 1,
	//		GL_RGB, GL_UNSIGNED_BYTE,
	//		formatedImages[i].imageData
	//	);
	//}


	//glGenerateMipmap(GL_TEXTURE_3D);
	//glBindTexture(GL_TEXTURE_3D, 0);
	
	
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
	//Create storage for the texture. (100 layers of 1x1 texels)
	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		1,                    //No mipmaps as textures are 1x1
		GL_RGBA8,              //Internal format
		width, height,                 //width,height
		depth                   //Number of layers
	);

	for (unsigned int i = 0 ;  i < formatedImages.size(); ++i)
	{
		//Specify i-essim image
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
			0,                     //Mipmap number
			0, 0, i,                 //xoffset, yoffset, zoffset
			formatedImages[i].imageWidth, formatedImages[i].imageHeight, 1,                 //width, height, depth
			GL_RGB,                //format
			GL_UNSIGNED_BYTE,      //type
			formatedImages[i].imageData);                //pointer to data
	}

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

	
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	// Return the ID of the texture we just created
	return textureID;
}

bool Texture::LoadTexture(const std::string& fileName, int& width, int& height, unsigned char ** data)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	BYTE* bits(0);

	GLuint gl_texID;
	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(fileName.c_str(), 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
	{
		fif = FreeImage_GetFIFFromFilename(fileName.c_str());
	}


	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
	{
		assert(false && "texture unknow file format");
	}


	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, fileName.c_str());

	height = FreeImage_GetHeight(dib);
	width = FreeImage_GetWidth(dib);
	int BPP = FreeImage_GetBPP(dib) / 8;
	bits = FreeImage_GetBits(dib);
	*data = (unsigned char *)malloc(width * height * BPP);
	memcpy(*data, bits, width * height * BPP);

	//if the image failed to load, return failure
	if (!dib)
	{
		assert(false && "Image failed to load 1");
	}

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

	return true;
}


void Texture::Bind(GLenum TextureUnit)
{
	glActiveTexture(GL_TEXTURE0 + TextureUnit);
	glBindTexture(myTextureTarget, texture_id);
}

GLuint Texture::GetTextureId()
{
	return texture_id;
}

bool Texture::GetIs2D()
{
	return myIs2D;
}

void Texture::SetIs2D(bool is2d)
{
	myIs2D = is2d;
}

