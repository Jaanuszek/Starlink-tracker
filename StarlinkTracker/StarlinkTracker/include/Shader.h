#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
	unsigned int vertexShaderID;
	unsigned int fragmentShaderID;
	unsigned int shaderProgramID;
public:
	// Shaders from variables
	Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
	// Shaders from files
	//Shader(const char* pathToVS, const char* pathToFS);
	~Shader();
	//void CreateShader();
	//void CompileShader();
	void useShaderProgram();
	void setUniformMat4fv(const std::string& name, const glm::mat4& matrix);
};

#endif