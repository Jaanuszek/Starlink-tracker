#include "../include/Texture.h"

Texture::Texture(const char* path)
    : ID(0), width(0), height(0), nrChannels(0), pathToTexture(path)
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

Texture::Texture(const std::vector<textureStruct>& texturesVector)
    : ID(0), width(0), height(0), nrChannels(0), textures(texturesVector)
{

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
void Texture::Bind(unsigned int textureIndex, unsigned int textureID)
{
    GLCall(glActiveTexture(GL_TEXTURE0 + textureIndex));
    GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
}
void Texture::BindAllTextures()
{
    if (!textures.empty())
    {
        for (unsigned int texIndex = 0; texIndex < textures.size(); texIndex++)
        {
            GLCall(glActiveTexture(GL_TEXTURE0 + texIndex));
            GLCall(glBindTexture(GL_TEXTURE_2D, textures[texIndex].ID));
        }
    }
    else
    {
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D, ID));
    }
}
void Texture::Unbind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::Unbind(unsigned int textureIndex, unsigned int textureID)
{
    GLCall(glActiveTexture(GL_TEXTURE0 + textureIndex));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::UnbindAllTextures()
{
    if (!textures.empty())
    {
        for (int texIndex = static_cast<int>(textures.size()) - 1; texIndex >= 0; texIndex--)
        {
            GLCall(glActiveTexture(GL_TEXTURE0 + texIndex));
            GLCall(glBindTexture(GL_TEXTURE_2D, 0));
        }
    }
    else
    {
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
}

unsigned int Texture::loadTextureFromFile(const char* path, const std::string& directory) {
	std::string filename = std::string(path);
    filename = directory + '/' + filename;
    unsigned int textureID;
    GLCall(glGenTextures(1, &textureID));
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
		GLenum format;
        if (nrChannels == 1)
			format = GL_RED;
        else if (nrChannels == 3)
			format = GL_RGB;
        else if (nrChannels == 4)
			format = GL_RGBA;

        GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
        GLCall(glGenerateMipmap(GL_TEXTURE_2D));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    }
    else {
        std::cout << "[ERROR] Failed to load texture: " << filename << std::endl;
    }
    stbi_image_free(data);
    return textureID;
}