#pragma once
#ifndef __TEXTURE__
#define __TEXTURE__
#include <stdint.h>
#include <iostream>
#include <GL\glew.h>

class Texture
{
public:
	Texture();
	Texture(const Texture& other) = delete;
	Texture& operator=(const Texture& other) = delete;

	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;
	~Texture();
	void createTexture(int w, int h, int bitDepth);
	void updateTexture(const void* texData);
	void loadTextureFromFile(const std::string& filename);
	void useTexture(GLuint unit=GL_TEXTURE0);
	void clearTexture();
private:
	GLuint texID;
	std::string texFileName;
	int32_t width;
	int32_t height;
	int32_t bitDepth;
};
#endif

