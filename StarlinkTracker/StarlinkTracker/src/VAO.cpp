#include "../include/VAO.h"

VAO::VAO() {
	GLCall(glGenVertexArrays(1, &ID));
}

VAO::~VAO() {
	std::cout << "[DEBUG] VAO deleted" << std::endl;
	GLCall(glDeleteVertexArrays(1, &ID));
}

void VAO::Bind() {
	GLCall(glBindVertexArray(ID));
}

void VAO::Unbind() {
	GLCall(glBindVertexArray(0));
}