#include "../include/Model.h"

void Model::loadModel(const char* pathToModel)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(pathToModel, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices |
        aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_ImproveCacheLocality | aiProcess_RemoveRedundantMaterials
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "[ERROR] ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directoryPath = std::string(pathToModel).substr(0, std::string(pathToModel).find_last_of('/'));

    processNode(scene->mRootNode, scene);

    addTexturesToUniformMap();

    std::cout << "[DEBUG] Model from path: " << pathToModel << " Loaded!" << std::endl;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }
    for (unsigned int childrenRoot = 0; childrenRoot < node->mNumChildren; childrenRoot++) {
        processNode(node->mChildren[childrenRoot], scene);
    }
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    vertices.reserve(mesh->mNumVertices);
    std::vector<unsigned int> indices;
    indices.reserve(mesh->mNumFaces * 3);
    std::vector<textureStruct> textures;
    for (unsigned int verticeIndex = 0; verticeIndex < mesh->mNumVertices; verticeIndex++)
    {
        Vertex vertex;
        glm::vec3 vector;
        vector.x = mesh->mVertices[verticeIndex].x;
        vector.y = mesh->mVertices[verticeIndex].y;
        vector.z = mesh->mVertices[verticeIndex].z;
        vertex.position = vector;
        vector.x = mesh->mNormals[verticeIndex].x;
        vector.y = mesh->mNormals[verticeIndex].y;
        vector.z = mesh->mNormals[verticeIndex].z;
        vertex.normal = vector;
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            // In case if we want to use different texutre we have to change this [0].
            vec.x = mesh->mTextureCoords[0][verticeIndex].x;
            vec.y = mesh->mTextureCoords[0][verticeIndex].y;
            vertex.texPos = vec;
        }
        else {
            vertex.texPos = glm::vec2(0.0f, 0.0f);
        }
        vertices.emplace_back(std::move(vertex));
    }

    for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++)
    {
        aiFace face = mesh->mFaces[faceIndex];
        for (unsigned int indiceIndex = 0; indiceIndex < face.mNumIndices; indiceIndex++)
        {
            indices.emplace_back(face.mIndices[indiceIndex]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<textureStruct> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<textureStruct> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    meshStructVector.push_back(meshStruct{ vertices, indices, textures});
}

void Model::addTexturesToUniformMap()
{
    int textureDiffuseIndex = 0;
    int textureSpecularIndex = 0;
    int textureIndex = 0;
    for (auto& texture : texturesLoaded)
    {
        shaderUniformData uniform;
        std::string textureType;
        if (texture.type == "texture_diffuse") {
            textureType = "texture_diffuse" + std::to_string(textureDiffuseIndex);
            uniform.data = textureIndex;
            textureDiffuseIndex++;
            textureIndex++;
        }
        else if (texture.type == "texture_specular") {
            textureType = "texture_specular" + std::to_string(textureSpecularIndex);
            uniform.data = textureIndex;
            textureDiffuseIndex++;
            textureIndex++;
        }
        else {
            textureType = texture.type;
        }

        uniform.uniformName = textureType;
        uniformDataMap[uniform.uniformName] = uniform;
    }
    UpdateShaderUniforms(uniformDataMap);
    shader.useShaderProgramWithUniformSet();
}

std::vector<textureStruct> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<textureStruct> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str); //get texture path
        bool skip = false;
        for (unsigned int j = 0; j < texturesLoaded.size(); j++)
        {
            if (std::strcmp(texturesLoaded[j].path, str.C_Str()) == 0)
            {
                textures.push_back(texturesLoaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            textureStruct texture;
            texture.ID = Texture::loadTextureFromFile(str.C_Str(), directoryPath);
            std::cout << "[DEBUG] Texture loaded: " << directoryPath << "/" << str.C_Str() << " with ID: " << texture.ID << std::endl;
            std::cout << "[DEBUG] Texture type: " << typeName << std::endl;
            texture.path = str.C_Str();
            texture.type = typeName;
            textures.push_back(texture);
            texturesLoaded.push_back(texture);
        }
    }
    return textures;
}

void Model::createMeshes()
{
    for (auto& meshData : meshStructVector)
    {
        objectsMeshes.emplace_back(meshData);
    }
}

Model::Model(const char* pathToModel, const char* pathToShader) : shader{pathToShader}
{
    loadModel(pathToModel);
    createMeshes();
}

Model::~Model() {
}

void Model::DrawModel() {
    // bind shaderProgram
    // set shader uniforms from map that is in Shader class
    // Then draw all meshes
    shader.useShaderProgramWithUniformSet();  // Use program shader, set uniforms, and draw
    for (auto& mesh : objectsMeshes)
    {
        mesh.Draw(GL_TRIANGLES);
    }
}

void Model::UpdateShaderUniforms(const std::unordered_map<std::string, shaderUniformData>& uniformData)
{
    for (const auto& [name, uniform] : uniformData)
    {
        shader.updateUniformMap(uniform);
    }
}

void Model::ActivateModelShader()
{
    shader.useShaderProgramWithUniformSet();
}