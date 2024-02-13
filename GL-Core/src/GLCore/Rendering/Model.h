#pragma once


#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"

#include <string>

#include <map>
#include <vector>
using namespace std;


class Model {
public:
    vector<Texture> textures_loaded;
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection = false;

    Model() = default;

    explicit Model(string const&path, const bool gamma = false) : gammaCorrection(gamma) {
        LoadModel(path);
    }

    void Draw(Shader&shader, const bool draw = false) const {
        for (auto&mesh: meshes)
            mesh.Draw(shader, draw);
    }

private:
    void LoadModel(string const&path);

    vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);


    void ProcessNode(const aiNode* node, const aiScene* scene);

    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
};

unsigned int TextureFromFile(const char* path, const string&directory, bool gamma = false);
