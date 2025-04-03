#ifndef VBO_H
#define VBO_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include "JSONParser.h"

struct VertexPosOnly; // Forward declaration

struct VertexAttrib {
    GLuint index;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLint stride;
    const void* pointer;
};

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texPos;
};

class VBO
{
private:
    unsigned int ID;
    std::vector<VertexAttrib> attributes;
public:
    VBO(float* vertices, GLsizeiptr size, const std::vector<VertexAttrib>& attribs);
    VBO(const std::vector<Vertex>& vertices);
    VBO(const std::vector<VertexPosOnly>& vertices, bool dynamicallyUpdated);
    ~VBO();
    void Bind();
    void Unbind();
    void AddVertexStructAttribs();
    void SetAttribPointers();
    void SetAttribPointers(const std::vector<VertexAttrib>& attribs);
    void UpdateData(const std::vector<VertexPosOnly>& vertices);
};

#endif // !VBO_H