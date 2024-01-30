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
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
};


class Mesh {
public:
    vector<Vertex> m_Vertives;
    vector<unsigned int> m_Indices;
    glm::vec3 m_Center;
    unsigned int VAO;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, glm::vec3 center);

    void Draw(Shader&shader, bool point = false);

private:
    unsigned int VBO, EBO;

    void SetupMesh();
};
