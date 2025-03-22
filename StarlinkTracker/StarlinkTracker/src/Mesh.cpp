#include "../include/Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const std::string& pathToTexture)
	: vbo(vertices), ebo(indices), texture(pathToTexture.c_str())
{
	// Bind VAO -> Bind VBO -> Bind EBO -> Set attrib pointers -> Unbind VBO ->Unbind VAO
	// Do not unbind EBO
	vao.Bind();
	vbo.Bind();
	ebo.Bind();
	vbo.AddVertexStructAttribs();
	vbo.SetAttribPointers();
	vbo.Unbind();
	vao.Unbind();

	texture.Bind(0);
}

Mesh::~Mesh()
{
	std::cout << "[DEBUG] Mesh deleted" << std::endl;
	vbo.Unbind();
	vao.Unbind();
}

void Mesh::Draw()
{
	vao.Bind();
	glDrawElements(GL_TRIANGLES, ebo.getCountBytes(), GL_UNSIGNED_INT, 0);
	vao.Unbind();
}