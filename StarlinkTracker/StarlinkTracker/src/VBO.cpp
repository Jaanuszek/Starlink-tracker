#include "../include/VBO.h"

VBO::VBO(float* vertices, GLsizeiptr size, const std::vector<VertexAttrib>& attribs) : attributes(attribs)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VBO::~VBO() {
	std::cout << "[DEBUG] VBO deleted" << std::endl;
	glDeleteBuffers(1, &ID);
}

void VBO::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::setAttribPointers() {
	for (const auto& attribute : attributes) {
		glEnableVertexAttribArray(attribute.index);
		glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized, attribute.stride, attribute.pointer);
	}
}