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
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection = false;

    Model() = default;

    explicit Model(string const&path, const bool gamma = false) : gammaCorrection(gamma) {
        LoadModel(path);
    }

    void Draw(Shader&shader, const bool draw = false) {
        for (auto&mesh: meshes)
            mesh.Draw(shader, draw);
    }

private:
    void LoadModel(string const&path);

    void ProcessNode(const aiNode* node, const aiScene* scene);

    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
};
