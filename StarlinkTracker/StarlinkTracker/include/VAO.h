#ifndef VAO_H
#define VAO_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "DebugLogs.h"

class VAO
{
private:
	unsigned int ID;
public:
	VAO();
	~VAO();
	void Bind();
	void Unbind();
};

#endif // !VAO_H
