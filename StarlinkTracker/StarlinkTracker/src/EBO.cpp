#include "../include/EBO.h"

EBO::EBO(std::vector<unsigned int>& indices) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	indicesCountBytes = indices.size() * sizeof(unsigned int);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	Unbind();
}

EBO::~EBO() {
	std::cout << "[DEBUG] EBO deleted" << std::endl;
	glDeleteBuffers(1, &ID);
}

void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}