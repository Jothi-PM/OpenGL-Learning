#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string>


Texture::Texture()
{
	width = 0;
	height = 0;
	bitDepth = 0;
	texID = 0;
	texFileName = "";
}

Texture::Texture(Texture&& other) noexcept
{
	texID = other.texID;
	width = other.width;
	height = other.height;
	bitDepth = other.bitDepth;
	texFileName = other.texFileName;

	other.texID = 0;
	other.width = 0;
	other.height = 0;
	other.bitDepth = 0;
	other.texFileName = "";
}

Texture& Texture::operator=(Texture&& other) noexcept
{
	if (this != &other)
	{
		clearTexture();

		texID = other.texID;
		width = other.width;
		height = other.height;
		bitDepth = other.bitDepth;
		texFileName = other.texFileName;

		other.texID = 0;
		other.width = 0;
		other.height = 0;
		other.bitDepth = 0;
		other.texFileName = "";
	}
	return *this;
}


void Texture::createTexture(int w, int h, int bitDepth)
{
GLenum dataFormat = (bitDepth == 4) ? GL_BGRA : GL_BGR;
GLenum internalFormat = (bitDepth == 4) ? GL_RGBA : GL_RGB;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	//NOTE: opencv uses BGR format by default
	this->width = w;
	this->height = h;
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);

}

void Texture::updateTexture(const void* texData)
{
	if (texData == NULL)
		return;
	GLenum dataFormat = (bitDepth == 4) ? GL_BGRA : GL_BGR;
	GLenum internalFormat = (bitDepth == 4) ? GL_RGBA : GL_RGB;
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);

}

void Texture::loadTextureFromFile(const std::string& filename)
{
	if (filename == "")
	{
		std::cerr << "Invalid texture filename\n";
		return;
	}

	clearTexture();

	//texFileName = filename;
	
	// this is because OpenGL expects image (0,0) at bottom left
	stbi_set_flip_vertically_on_load(true);

	unsigned char* texData = stbi_load(filename.c_str(), &width, &height, &bitDepth, 0);
	if (texData == NULL)
	{
		std::cerr << "texture load failed\n";
		return;
	}

	GLenum format = (bitDepth == 4) ? GL_RGBA : GL_RGB;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(texData);

}

void Texture::useTexture(GLuint unit)
{
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, texID);
}


void Texture::clearTexture()
{
	if (texID != 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDeleteTextures(1, &texID);
		texID = 0;
	}
}

Texture::~Texture()
{
	clearTexture();
}
