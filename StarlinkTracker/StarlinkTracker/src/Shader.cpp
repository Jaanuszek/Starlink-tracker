#include "../include/Shader.h"

Shader::Shader(const char* pathToShader) : shaderFilePath(pathToShader), shaderProgramID(0)
{
	ShaderProgramSource shaderSource = parseShader(pathToShader);
	shaderProgramID = CreateShader(shaderSource.vertexShaderSource, shaderSource.fragmentShaderSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(shaderProgramID));
	std::cout << "[DEBUG] Shader program deleted" << std::endl;
}
ShaderProgramSource Shader::parseShader(const char* pathToShader) {
	std::ifstream file(pathToShader);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	if (!file.is_open()) {
		std::cerr << "[ERROR] Failed to open shader file" << std::endl;
		return {};
	}

	std::string line;
	std::stringstream stringStream[2];
	ShaderType type = ShaderType::NONE;
	while (getline(file, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			stringStream[(int)type] << line << '\n';
		}
	}
	return { stringStream[0].str(), stringStream[1].str() };
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int programID = glCreateProgram();
	unsigned int vertexShaderID = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	char infoLog[512];
	int success;
	GLCall(glAttachShader(programID, vertexShaderID));
	GLCall(glAttachShader(programID, fragmentShaderID));
	GLCall(glLinkProgram(programID));
	GLCall(glValidateProgram(programID));
	GLCall(glGetProgramiv(programID, GL_LINK_STATUS, &success));
	if (!success)
	{
		GLCall(glGetProgramInfoLog(programID, 512, NULL, infoLog));
		std::cerr << "[ERROR] Shader linking failed\n" << infoLog << std::endl;
		std::cerr << "[ERROR] Path to failed shader" << shaderFilePath << std::endl;
	}
	GLCall(glDeleteShader(vertexShaderID));
	GLCall(glDeleteShader(fragmentShaderID));
	return programID;
}
unsigned int Shader::CompileShader(GLenum shaderType, const std::string& shader)
{
	unsigned int shaderID = glCreateShader(shaderType);
	const char* shaderSource = shader.c_str();
	GLCall(glShaderSource(shaderID, 1, &shaderSource, NULL));
	GLCall(glCompileShader(shaderID));

	int success;
	char infoLog[512];
	GLCall(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GLCall(glGetShaderInfoLog(shaderID, 512, NULL, infoLog));
		std::cerr << "[ERROR] Shader compilation failed\n" << std::endl;
		std::cerr << "[ERROR] Shader type: " << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cerr << "[ERROR] Compile error log:\n" << infoLog << std::endl;
		GLCall(glDeleteShader(shaderID));
		return 0;
	}
	return shaderID;
}
void Shader::useShaderProgram()
{
	GLCall(glUseProgram(shaderProgramID));
}
void Shader::setUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value));
}

void Shader::setUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value));
}

void Shader::setUniformMat4fv(const std::string& name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix)));
}

void Shader::setUniformVec3f(const std::string& name, const glm::vec3& vector)
{
	GLCall(glUniform3fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, glm::value_ptr(vector)));
}

void Shader::setUniformVec4f(const std::string& name, const glm::vec4& vector)
{
	GLCall(glUniform4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, glm::value_ptr(vector)));
}