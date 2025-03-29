#ifndef EBO_H
#define EBO_H

#include <iostream>
#include <glad/glad.h>
#include <vector>

class EBO 
{
private:
	unsigned int ID;
	unsigned int indicesCountBytes;
public:
	EBO(std::vector<unsigned int>& indices);
	EBO() = default;
	~EBO();
	void Bind();
	void Unbind();
	unsigned int getCountBytes() const { return indicesCountBytes; }
};

#endif