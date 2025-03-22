#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <string>
#include "stb/stb_image.h"

class Texture {
private:
	unsigned int ID;
	int width, height, nrChannels;
	unsigned char* textureData = nullptr;
public:
	Texture(const char* path);
	~Texture();

	void Bind(unsigned int textureIndex);
	void Unbind();
};

#endif