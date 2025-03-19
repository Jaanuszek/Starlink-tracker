#include "../include/VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &ID);
}

VAO::~VAO() {
	std::cout << "[DEBUG] VAO deleted" << std::endl;
	glDeleteVertexArrays(1, &ID);
}

void VAO::Bind() {
	glBindVertexArray(ID);
}

void VAO::Unbind() {
	glBindVertexArray(0);
}