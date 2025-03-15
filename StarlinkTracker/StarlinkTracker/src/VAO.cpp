#include "../include/VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &ID);
}

VAO::~VAO() {
	std::cout << "[DEBUG] VAO deleted" << std::endl;
	glDeleteVertexArrays(1, &ID);
}

void VAO::bind() {
	glBindVertexArray(ID);
}

void VAO::unbind() {
	glBindVertexArray(0);
}