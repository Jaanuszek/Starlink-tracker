#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "JSONParser.h"
#include "DebugLogs.h"
#include "Shader.h"

// Struct that contains all the mesh informations vertices, indices and textures
struct meshStruct
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<textureStruct> textures;
};

class Mesh
{
private:
    VAO vao;
    VBO vbo;
    EBO ebo;
    Texture texture;
    std::vector<textureStruct> textures;
    void BindAllTextures();
    void UnbindAllTextures();
public:
    Mesh() : vao(), vbo(), ebo(), texture() {}
    // Mesh with signle texture
    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const std::string& pathToTexture);
    // Mesh with multiple textures
    Mesh(meshStruct& meshData);
    Mesh(std::vector<VertexPosOnly>& vertices, bool dynamicUpdate = false);
    Mesh(std::map<unsigned int, std::vector<VertexPosOnly>>& vertices);
    ~Mesh();
    void Draw(GLenum primitiveType);
    void DrawWithoutEBO(GLenum primitiveType, unsigned int count);
    void DrawMultipleMeshes(GLenum primitiveType, std::vector<int> firstIndices, std::vector<int> counts, GLsizei drawCount);
    void UpdateData(const std::vector<VertexPosOnly>& vertices);
};

#endif