#include "../include/VBO.h"

VBO::VBO(float* vertices, GLsizeiptr size, const std::vector<VertexAttrib>& attribs) : attributes(attribs)
{
    GLCall(glGenBuffers(1, &ID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
    Unbind();
}

VBO::VBO(const std::vector<Vertex>& vertices) {
    GLCall(glGenBuffers(1, &ID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW));
}

VBO::VBO(const std::vector<VertexPosOnly>& vertices, bool dynamicallyUpdated) {
    if (dynamicallyUpdated == false)
    {
        GLCall(glGenBuffers(1, &ID));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
        GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexPosOnly), vertices.data(), GL_STATIC_DRAW));
    }
    else
    {
        GLCall(glGenBuffers(1, &ID));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
        GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexPosOnly), vertices.data(), GL_DYNAMIC_DRAW));
    }
}

VBO::~VBO() {
    std::cout << "[DEBUG] VBO deleted" << std::endl;
    GLCall(glDeleteBuffers(1, &ID));
}

void VBO::Bind() {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
}

void VBO::Unbind() {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VBO::AddVertexStructAttribs() {
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)));
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texPos)));
}

void VBO::SetAttribPointers() {
    for (const auto& attribute : attributes) {
        GLCall(glEnableVertexAttribArray(attribute.index));
        GLCall(glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized, attribute.stride, attribute.pointer));
    }
}

void VBO::SetAttribPointers(const std::vector<VertexAttrib>& attribs) {
    for (const auto& attribute : attribs) {
        GLCall(glEnableVertexAttribArray(attribute.index));
        GLCall(glVertexAttribPointer(attribute.index, attribute.size, attribute.type, attribute.normalized, attribute.stride, attribute.pointer));
    }
}

void VBO::UpdateData(const std::vector<VertexPosOnly>& vertices) {
    Bind();
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(VertexPosOnly), vertices.data()));
    Unbind();
}
