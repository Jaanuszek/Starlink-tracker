#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <string>
#include "stb/stb_image.h"
#include "DebugLogs.h"

struct textureStruct
{
    unsigned int ID;
	const char* path;
	std::string type;
};

class Texture {
private:
	unsigned int ID;
	int width, height, nrChannels;
	const char* pathToTexture;
    //std::string textureType;
	unsigned char* textureData = nullptr;
public:
	Texture(const char* path);
	Texture() = default;
	~Texture();

	void Bind(unsigned int textureIndex);
    void Bind(unsigned int textureIndex, unsigned int textureID);
	void Unbind();
    void Unbind(unsigned int textureIndex, unsigned int textureID);
	// :this "texture_diffuse" is temporary solution
    textureStruct getTextureStruct() const { return { ID, pathToTexture, "texture_diffuse"}; }
    static unsigned int loadTextureFromFile(const char* path, const std::string& directory);
};

#endif