#include "FBO.h"
#include <glad/glad.h>
#include <cassert>

void Fbo::CreateFBO(const int w, const int h)
{
	width = w;
	height = h;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Create a render buffer, and attach it to FBO's depth attachment
	unsigned int depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
		width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
		GL_RENDERBUFFER, depthBuffer);

	// Create texture and attach FBO's color 0 attachment
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
		0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, texture, 0);

	// Check for completeness/correctness
	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		assert(false && "FBO error");
	}
		
	// Unbind the fbo.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Fbo::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Fbo::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
