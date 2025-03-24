#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "JSONParser.h"

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
	Mesh(std::vector<VertexPosOnly>& vertices);
	~Mesh();
	void Draw(GLenum primitiveType);
	void DrawWithoutEBO(GLenum primitiveType, unsigned int count);
};

#endif