#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <string>
#include <vector>
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
	unsigned char* textureData = nullptr;
    std::vector<textureStruct> textures;
public:
	Texture(const char* path);
    // Constructor for multiple textures f.e. models
	Texture(const std::vector<textureStruct>& texturesVector);
	Texture() = default;
	~Texture();

	void Bind(unsigned int textureIndex);
    void Bind(unsigned int textureIndex, unsigned int textureID);
	void BindAllTextures();
	void Unbind();
    void Unbind(unsigned int textureIndex, unsigned int textureID);
    void UnbindAllTextures();
    static unsigned int loadTextureFromFile(const char* path, const std::string& directory);
};

#endif