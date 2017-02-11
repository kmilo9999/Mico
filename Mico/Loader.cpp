#include "Loader.h"

Loader* Loader::instance(0);


Loader * Loader::GetInstance()
{
	if (!instance)
	{
		instance = new Loader();
	}
	return instance;
}

Loader::~Loader()
{
}

RawModel* Loader::LoadToVAO(const vector<vec3>& positions, const vector<vec2>& textCoodinates, const vector<vec3>& normals, const vector<int>& indexes)
{
	int vao = CreateVAO();
	StoreDataInAttributeList(0, 3, positions);
	StoreDataInAttributeList(1, 2, textCoodinates);
	StoreDataInAttributeList(2, 3, normals);
	BindIndicesBuffer(indexes);
	UnbindVAO();


	return  new RawModel(vao, (int)indexes.size());
}

int Loader::LoadTexture(const char * fileName)
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
	fif = FreeImage_GetFileType(fileName, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(fileName);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, fileName);

	//if the image failed to load, return failure
	if (!dib)
		return false;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
		return false;

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

	textures.push_back(textureID);

	// Return the ID of the texture we just created
	return textureID;
}

int Loader::CreateVAO()
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	vaos.push_back(vao);
	glBindVertexArray(vao);

	return vao;
}

void Loader::StoreDataInAttributeList(int attribute, int coordinateSize, vector<vec3> data)
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	vbos.push_back(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(vec3),
		&data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, coordinateSize, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::StoreDataInAttributeList(int attribute, int coordinateSize, vector<vec2>  data)
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	vbos.push_back(vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(vec2),
		&data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, coordinateSize, GL_FLOAT, GL_FALSE, sizeof(vec2), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Loader::BindIndicesBuffer(vector<int> indexes)
{
	GLuint ebo;
	glGenBuffers(1, &ebo);
	vbos.push_back(ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(int),
		&indexes[0], GL_STATIC_DRAW);
}

void Loader::UnbindVAO()
{
	glBindVertexArray(0);
}


void Loader::CleanUp()
{
	if(vaos.size())
		glDeleteVertexArrays((int)vaos.size(), &vaos[0]);
	
	if(vbos.size())
		glDeleteVertexArrays((int)vbos.size(),&vbos[0]);
	
	if(textures.size())
		glDeleteTextures((int)textures.size(), &textures[0]);
}