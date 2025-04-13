#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "VBO.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
private:
    std::string directoryPath;
    std::vector<Mesh> objectsMeshes;
    std::vector<meshStruct> meshStructVector;
    std::vector<textureStruct> texturesLoaded;
    std::unordered_map<std::string, shaderUniformData> uniformDataMap;
    void loadModel(const char* pathToModel);
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);
    void createMeshes();
    Shader shader;
    std::vector<textureStruct> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
public:
    Model(const char* pathToModel, const char* pathToShader);
    ~Model();
    void DrawModel();
    void UpdateShaderUniforms(const std::unordered_map<std::string, shaderUniformData>& uniformData);
    void ActivateModelShader();
};

#endif