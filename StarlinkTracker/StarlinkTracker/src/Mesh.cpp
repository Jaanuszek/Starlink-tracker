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
	//vbo.SetAttribPointers();
	vbo.Unbind();
	vao.Unbind();

	texture.Bind(0);
}

Mesh::Mesh(std::vector<VertexPosOnly>& vertices)
	: vbo(vertices), ebo(), texture()
{
	std::vector<VertexAttrib> attribs = {
		{0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPosOnly), (void*)0}
	};
	vao.Bind();
	vbo.Bind();
	vbo.SetAttribPointers(attribs);
	vbo.Unbind();
	vao.Unbind();
}

Mesh::~Mesh()
{
	std::cout << "[DEBUG] Mesh deleted" << std::endl;
	vbo.Unbind();
	vao.Unbind();
}

void Mesh::Draw(GLenum primitiveType)
{
	vao.Bind();
	glDrawElements(primitiveType, ebo.getCountBytes(), GL_UNSIGNED_INT, 0);
	vao.Unbind();
}

void Mesh::DrawWithoutEBO(GLenum primitiveType, unsigned int count)
{
	vao.Bind();
	glDrawArrays(primitiveType, 0, count);
	vao.Unbind();
}

void Mesh::DrawMultipleMeshes(GLenum primitiveType, std::vector<int> firstIndices, std::vector<int> counts, GLsizei drawCount)
{
	vao.Bind();
	glMultiDrawArrays(primitiveType, firstIndices.data(), counts.data(), drawCount);
	vao.Unbind();
}