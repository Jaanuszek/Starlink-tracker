#ifndef VAO_H
#define VAO_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class VAO
{
private:
	unsigned int ID;
public:
	VAO();
	~VAO();
	void bind();
	void unbind();
};

#endif // !VAO_H
