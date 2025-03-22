#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"

// TODO add camera and textures here

class Mesh
{
private:
	VAO vao;
	VBO vbo;
	EBO ebo;
	Texture texture;
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const std::string& pathToTexture);
	~Mesh();
	void Draw();
};

#endif