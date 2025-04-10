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
    std::vector<std::pair<std::vector<Vertex>, std::vector<unsigned int>>> meshes;
    std::vector<Mesh> objectsMeshes;
    std::vector<textureStruct> texturesLoaded;
    void loadModel(const char* pathToModel);
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);
    void createMeshes();
    std::vector<textureStruct> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
public:
    Model(const char* pathToModel);
    ~Model();
    void DrawModel();
};

#endif