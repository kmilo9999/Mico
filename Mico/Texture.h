#pragma once
#include <glad\glad.h>
#include <FreeImage\FreeImage.h>
#include <string>
#include <vector>
class Texture
{

public:

	//! Loads a 2D textures
	Texture(GLenum TextureTarget, const std::string& filename, bool is2D = true);

	//! Loads an array of 2D textues into a 3D texture
	Texture(GLenum TextureTarget, std::vector<std::string> paths, bool is2D = false);

	~Texture();


	void Bind(GLenum TextureUnit);
	GLuint GetTextureId();

	std::string type;

	bool GetIs2D();
	void SetIs2D(bool is2d);

private:

	struct image
	{
		image() : imageWidth(0), imageHeight(0), imageData(0) {};
		image(int w, int h, unsigned char* data) 
			: imageWidth(w), imageHeight(h), imageData(data) {};
		int imageWidth;
		int imageHeight;
		unsigned char* imageData;
	};

	int Load2DTexture();
	int Load3DTexture(const std::vector<std::string>& paths);
	bool LoadTexture(const std::string& fileName, int& width, int& height, unsigned char ** data);
	GLuint texture_id;
	GLenum myTextureTarget;
	const char* myFileName;
	bool myIs2D;
};

