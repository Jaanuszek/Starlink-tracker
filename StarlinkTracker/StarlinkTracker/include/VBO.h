#ifndef VBO_H
#define VBO_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

struct VertexAttrib {
	GLuint index;
	GLint size;
	GLenum type;
	GLboolean normalized;
	GLint stride;
	const void* pointer;
};

class VBO
{
private:
	unsigned int ID;
	std::vector<VertexAttrib> attributes;
public:
	VBO(float* vertices, GLsizeiptr size, const std::vector<VertexAttrib>& attribs);
	~VBO();
	void bind();
	void unbind();
	void setAttribPointers();
};

#endif // !VBO_H