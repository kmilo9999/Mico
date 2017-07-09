#pragma once
class Fbo
{
public:
	unsigned int fbo;

	unsigned int texture;
	int width, height;  // Size of the texture.

	void CreateFBO(const int w, const int h);
	void Bind();
	void Unbind();
};

