#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <variant>
#include <typeinfo>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "DebugLogs.h"

enum class WhichVector
{
	VECTOR_FROM_CLASS_POLE,
	VECTOR_FROM_AGRUMENT
};

enum class uniformType
{
    UNIFORM_1D_INT,
    UNIFORM_1D_FLOAT,
    UNIFORM_VEC3_FLOAT,
    UNIFORM_VEC4_FLOAT,
    UNIFORM_MAT4_FV // float, pointer
};

struct shaderUniformData
{
    //uniformType type;
	std::string uniformName;
	// if you want to add another uniform variable types,
	// just add it in this std::variant,
	// and also in Shader.cpp add another if statement in "setUniformFromStruct" function
    std::variant<int, float, glm::vec3, glm::vec4, glm::mat4> data;
};

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
	std::vector<shaderUniformData> uniformDataVector;
	ShaderProgramSource parseShader(const char* pathToShader);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(GLenum shaderType, const std::string& shader);
public:
	Shader(const char* pathToShader,const std::vector<shaderUniformData>& uniformData);
	~Shader();
	void useShaderProgram();
	void useShaderProgramWithUniformSet();
    void setMultipleUniformsFromStruct();
    void setUniformFromStruct(const shaderUniformData& uniform);
	// I would change this setter to change or add non existing values, for example
	// If we store vector with 4 elements, and we pass anothre vector to this setter,
	// We don't want to overwrite this vector that we are storing, it may lead to some bad things
	// That's why I should add some kind of cheker? overwriter? 
	void setUniformDataVector(std::vector<shaderUniformData>& uniformData) { uniformDataVector = uniformData; }
	void setUniform1f(const std::string& name, float value);
	void setUniform1i(const std::string& name, int value);
	void setUniformMat4fv(const std::string& name, const glm::mat4& matrix);
    void setUniformVec3f(const std::string& name, const glm::vec3& vector);
    void setUniformVec4f(const std::string& name, const glm::vec4& vector);
};

#endif