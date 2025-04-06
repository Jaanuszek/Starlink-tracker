#include "../include/EBO.h"

EBO::EBO(std::vector<unsigned int>& indices) {
	GLCall(glGenBuffers(1, &ID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
	indicesCountBytes = indices.size() * sizeof(unsigned int);
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW));
	Unbind();
}

EBO::~EBO() {
	std::cout << "[DEBUG] EBO deleted" << std::endl;
	GLCall(glDeleteBuffers(1, &ID));
}

void EBO::Bind() {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
}

void EBO::Unbind() {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}