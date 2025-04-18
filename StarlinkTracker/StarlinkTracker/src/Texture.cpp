#include "../include/Texture.h"

Texture::Texture(const char* path) : ID(0), width(0), height(0), nrChannels(0)
{
	stbi_set_flip_vertically_on_load(1);
	textureData = stbi_load(path, &width, &height, &nrChannels, 4);
	if (textureData)
	{
		GLCall(glGenTextures(1, &ID));
		GLCall(glBindTexture(GL_TEXTURE_2D, ID));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
	else
	{
		std::cout << "[ERROR] Something is wrong with the texture. Path: " << path << std::endl;
	}
	stbi_image_free(textureData);
}

Texture::~Texture()
{
	std::cout << "[DEBUG] Texture deleted" << std::endl;
	GLCall(glDeleteTextures(1, &ID));
}

void Texture::Bind(unsigned int textureIndex)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + textureIndex));
	GLCall(glBindTexture(GL_TEXTURE_2D, ID));
}

void Texture::Unbind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}