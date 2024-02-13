#pragma once


#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

#include <string>
#include <vector>
using namespace std;


struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};


class Mesh {
public:
    vector<Vertex> m_Vertives;
    vector<unsigned int> m_Indices;
    vector<Texture> m_Textures;
    glm::vec3 m_Center;
    unsigned int VAO;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, glm::vec3 center);

    void Draw(Shader&shader, bool point = false) const;

private:
    unsigned int VBO, EBO;

    void SetupMesh();
};
