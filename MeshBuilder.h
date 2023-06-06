#pragma once

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <gl/glew.h>
#include "Mesh.h"

class MeshBuilder
{
public:
    MeshBuilder();
    ~MeshBuilder();

    GLuint addVertex(glm::vec3 vertex, glm::vec3 normal, glm::vec2 uv);
    void addTriangle(GLuint v1, GLuint v2, GLuint v3);
    Mesh* buildMesh();


private:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> uvs;
    std::vector<GLuint> indices;

    GLuint vertexIndex = 0;
};

