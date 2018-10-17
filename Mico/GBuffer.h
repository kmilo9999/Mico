#pragma once
#include <glad\glad.h>
#include <vector>

class GBuffer
{
public:

	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_TEXTURE_TYPE_DIFFUSE,
		GBUFFER_TEXTURE_TYPE_SPECULAR,
		GBUFFER_TEXTURE_TYPE_SHININESMAP,
		GBUFFER_TEXTURE_TYPE_TEXCOORD,
		GBUFFER_NUM_TEXTURES
	};

	GBuffer();

	~GBuffer();

	bool CreateGBuffer(unsigned int WindowWidth, unsigned int WindowHeight);

	void BindForWriting();

	void BindForReading();

	GLuint GetTextureUnit(GBUFFER_TEXTURE_TYPE texture);

	GLuint GetDepthTexture();


private:

	GLuint m_fbo;
	GLuint m_textures[GBUFFER_NUM_TEXTURES];
	GLuint m_depthTexture;
};

