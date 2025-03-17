#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ShaderProgramSource
{
	std::string vertexShaderSource;
	std::string fragmentShaderSource;
};

class Shader
{
private:
	unsigned int shaderProgramID;
	const char* shaderFilePath;
	ShaderProgramSource parseShader(const char* pathToShader);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(GLenum shaderType, const std::string& shader);
public:
	Shader(const char* pathToShader);
	~Shader();
	void useShaderProgram();
	void setUniformMat4fv(const std::string& name, const glm::mat4& matrix);
};

#endif