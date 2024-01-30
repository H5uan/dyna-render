#include "Model.h"
#include <filesystem>
namespace fs = std::filesystem;

void Model::LoadModel(const std::string&path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
              aiProcess_GenBoundingBoxes);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOG_ERROR("ERROR::ASSIMP:: {0}\n", importer.GetErrorString());
        return;
    }

    const fs::path filepath = path;
    directory = filepath.parent_path().string();

    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(const aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex{};
        glm::vec3 vector = {
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        };

        vertex.Position = vector;

        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        vertices.emplace_back(vertex);
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.emplace_back(face.mIndices[j]);
    }
    auto center = glm::vec3(
        (mesh->mAABB.mMin.x + mesh->mAABB.mMax.x) / 2.0f,
        (mesh->mAABB.mMin.y + mesh->mAABB.mMax.y) / 2.0f,
        (mesh->mAABB.mMin.z + mesh->mAABB.mMax.z) / 2.0f
    );

    return {vertices, indices, center};
}
