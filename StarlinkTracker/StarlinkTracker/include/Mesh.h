#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

// TODO add camera and textures here

class Mesh
{
private:
	VAO vao;
	VBO vbo;
	EBO ebo;
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	~Mesh();
	void Draw();
};

#endif