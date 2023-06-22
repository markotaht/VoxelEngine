#pragma once

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <gl/glew.h>
#include "Mesh.h"

class MeshBuilder
{
public:
    MeshBuilder();
    MeshBuilder(int vertexCount, int indexCount);
    ~MeshBuilder();

//    GLuint addVertex(glm::vec3 vertex, glm::vec3 normal, glm::vec2 uv);
    GLuint addVertex(glm::vec3 vertex, glm::vec3 normal, glm::vec3 uv);
    void addTriangle(GLuint v1, GLuint v2, GLuint v3);
    Mesh* buildMesh();


private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> uvs;
    std::vector<GLuint> indices;

    GLuint vertexIndex = 0;
};

