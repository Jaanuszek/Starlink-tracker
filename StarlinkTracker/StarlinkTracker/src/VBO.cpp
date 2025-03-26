#include "../include/VBO.h"

VBO::VBO(float* vertices, GLsizeiptr size, const std::vector<VertexAttrib>& attribs) : attributes(attribs)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	Unbind();
	//SetAttribPointers();
}

VBO::VBO(const std::vector<Vertex>& vertices) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	//AddVertexStructAttribs();
}

VBO::VBO(const std::vector<VertexPosOnly>& vertices) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexPosOnly), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPosOnly), (void*)0);
}

VBO::~VBO() {
	std::cout << "[DEBUG] VBO deleted" << std::endl;
	glDeleteBuffers(1, &ID);
}

void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::AddVertexStructAttribs() {
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texPos));
}

void VBO::SetAttribPointers() {
	for (const auto& attribute : attributes) {
		glEnableVertexAttribArray(attribute.index);
		glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized, attribute.stride, attribute.pointer);
	}
}

void VBO::SetAttribPointers(const std::vector<VertexAttrib>& attribs) {
	for (const auto& attribute : attribs) {
		glEnableVertexAttribArray(attribute.index);
		glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized, attribute.stride, attribute.pointer);
	}
}
