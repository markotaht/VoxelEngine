#include "NewMesh.h"
#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include <gl/glew.h>
#include <iostream>

namespace engine {
	namespace asset {
		void Mesh::upload()
		{
			indexCount = static_cast<GLsizei>(indices.size());
			if (indexCount == 0) return;

			initBuffers();

			glBindVertexArray(vao);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vboPositions);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

			if (normals.size() > 0) {
				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
				glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			}

			if (uvType == UVType::Vec2 && uvs2D.size() > 0) {
				glEnableVertexAttribArray(2);
				glBindBuffer(GL_ARRAY_BUFFER, vboUVs);
				glBufferData(GL_ARRAY_BUFFER, uvs2D.size() * sizeof(glm::vec2), uvs2D.data(), GL_STATIC_DRAW);
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
			}
			else if (uvType == UVType::Vec3 && uvs3D.size() > 0) {
				glEnableVertexAttribArray(2);
				glBindBuffer(GL_ARRAY_BUFFER, vboUVs);
				glBufferData(GL_ARRAY_BUFFER, uvs3D.size() * sizeof(glm::vec3), uvs3D.data(), GL_STATIC_DRAW);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			}

			if (colors.size() > 0) {
				glEnableVertexAttribArray(3);
				glBindBuffer(GL_ARRAY_BUFFER, vboColors);
				glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
				glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
			
			glBindVertexArray(0);
		}

		void Mesh::upload(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec3> colors, std::vector<glm::vec2> uv2D, std::vector<uint32_t> indices)
		{
			this->vertices = std::move(vertices);
			this->normals = std::move(normals);
			this->colors = std::move(colors);
			this->uvs2D = std::move(uv2D);
			this->indices = std::move(indices);
			uvType = UVType::Vec2;
			upload();
		}

		void Mesh::upload(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec3> colors, std::vector<uint32_t> indices)
		{
			this->vertices = std::move(vertices);
			this->normals = std::move(normals);
			this->colors = std::move(colors);
			this->indices = std::move(indices);
			uvType = UVType::Vec2;
			upload();
		}

		void Mesh::upload(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec3> colors, std::vector<glm::vec3> uv3D, std::vector<uint32_t> indices)
		{
			this->vertices = std::move(vertices);
			this->normals = std::move(normals);
			this->colors = std::move(colors);
			this->uvs3D = std::move(uv3D);
			this->indices = std::move(indices);
			uvType = UVType::Vec3;
			upload();
		}

		void Mesh::unload()
		{
			if (vao) {
				glDeleteVertexArrays(1, &vao);
				vao = 0;
			}
			if (vboPositions) {
				glDeleteBuffers(1, &vboPositions);
				vboPositions = 0;
			}
			if (vboNormals) {
				glDeleteBuffers(1, &vboNormals);
				vboNormals = 0;
			}
			if (vboColors) {
				glDeleteBuffers(1, &vboColors);
				vboColors = 0;
			}
			if (vboUVs) {
				glDeleteBuffers(1, &vboUVs);
				vboUVs = 0;
			}
			if (ibo) {
				glDeleteBuffers(1, &ibo);
				ibo = 0;
			}

			indexCount = 0;
			uvType = UVType::Vec2;

			// Optional
			vertices.clear();
			normals.clear();
			uvs2D.clear();
			uvs3D.clear();
			colors.clear();
			indices.clear();
		}

		void Mesh::bind()
		{
			glBindVertexArray(vao);
		}

		void Mesh::render()
		{
			glDrawElements(renderType, indexCount, GL_UNSIGNED_INT, 0);
		}

		std::unique_ptr<Mesh> Mesh::makeCube()
		{
			std::vector<glm::vec3> vertices = {
				//BACK
				glm::vec3{-1.f,-1.f,-1.f},
				glm::vec3{1.f,-1.f,-1.f},
				glm::vec3{1.f,1.f,-1.f},
				glm::vec3{-1.f,1.f,-1.f},

				//FRONT
				glm::vec3{-1.f,-1.f,1.f},
				glm::vec3{-1.f,1.f,1.f},
				glm::vec3{1.f,1.f,1.f},
				glm::vec3{1.f,-1.f,1.f},

				//TOP
				glm::vec3{-1.f,1.f,-1.f},
				glm::vec3{-1.f,1.f,1.f},
				glm::vec3{1.f,1.f,1.f},
				glm::vec3{1.f,1.f,-1.f},

				//BOTTOM
				glm::vec3{-1.f,-1.f,-1.f},
				glm::vec3{-1.f,-1.f,1.f},
				glm::vec3{1.f,-1.f,1.f},
				glm::vec3{1.f,-1.f,-1.f},

				//RIGHT
				glm::vec3{1.f,-1.f,-1.f},
				glm::vec3{1.f,1.f,-1.f},
				glm::vec3{1.f,1.f,1.f},
				glm::vec3{1.f,-1.f,1.f},

				//LEFT
				glm::vec3{-1.f,-1.f,-1.f},
				glm::vec3{-1.f,1.f,-1.f},
				glm::vec3{-1.f,1.f,1.f},
				glm::vec3{-1.f,-1.f,1.f}
			};

			std::vector<uint32_t> indices = {
				0,1,2, 0,2,3,
				4,5,6, 4,6,7,
				8,9,10, 8,10,11,
				12,13,14, 12,14,15,
				16,17,18, 16,18,19,
				20,21,22, 20,22,23
			};

			std::vector<glm::vec2> uvs = {
				glm::vec2{0.0f, 0.0f},
				glm::vec2{0.0f, 1.0f},
				glm::vec2{1.0f, 1.0f},
				glm::vec2{1.0f, 0.0f},

				glm::vec2{0.0f, 0.0f},
				glm::vec2{0.0f, 1.0f},
				glm::vec2{1.0f, 1.0f},
				glm::vec2{1.0f, 0.0f},

				glm::vec2{0.0f, 0.0f},
				glm::vec2{0.0f, 1.0f},
				glm::vec2{1.0f, 1.0f},
				glm::vec2{1.0f, 0.0f},

				glm::vec2{0.0f, 0.0f},
				glm::vec2{0.0f, 1.0f},
				glm::vec2{1.0f, 1.0f},
				glm::vec2{1.0f, 0.0f},

				glm::vec2{0.0f, 0.0f},
				glm::vec2{0.0f, 1.0f},
				glm::vec2{1.0f, 1.0f},
				glm::vec2{1.0f, 0.0f},

				glm::vec2{0.0f, 0.0f},
				glm::vec2{0.0f, 1.0f},
				glm::vec2{1.0f, 1.0f},
				glm::vec2{1.0f, 0.0f},
			};

			std::vector<glm::vec3> normals = {
				//BACK
				glm::vec3{0, 0, -1},
				glm::vec3{0, 0, -1},
				glm::vec3{0, 0, -1},
				glm::vec3{0, 0, -1},

				//FRONT
				glm::vec3{0, 0, 1},
				glm::vec3{0, 0, 1},
				glm::vec3{0, 0, 1},
				glm::vec3{0, 0, 1},

				//TOP
				glm::vec3{0, 1,0},
				glm::vec3{0, 1,0},
				glm::vec3{0, 1,0},
				glm::vec3{0, 1,0},

				//BOTTOM
				glm::vec3{0, -1,0},
				glm::vec3{0, -1,0},
				glm::vec3{0, -1,0},
				glm::vec3{0, -1,0},

				//RIGHT
				glm::vec3{ 1,  0, 0},
				glm::vec3{1,  0, 0},
				glm::vec3{1,  0, 0},
				glm::vec3{1,  0, 0},

				//LEFT
				glm::vec3{ -1,  0, 0},
				glm::vec3{ -1,  0, 0},
				glm::vec3{-1,  0, 0},
				glm::vec3{1,  0, 0},
			};

			std::unique_ptr<Mesh> cubeMesh = std::make_unique<Mesh>();
			cubeMesh->upload(vertices, normals, {}, uvs, indices);
			return cubeMesh;
		}

		std::unique_ptr<Mesh> Mesh::makeAxis(){
			std::vector<glm::vec3> vertices = {
				//BACK
				glm::vec3(0,0,0),
				glm::vec3(1,0,0),

				glm::vec3(0,0,0),
				glm::vec3(0,1,0),

				glm::vec3(0,0,0),
				glm::vec3(0,0,1),
			};

			std::vector<glm::vec3> colors = {
				//BACK
				glm::vec3(1,0,0),
				glm::vec3(1,0,0),

				glm::vec3(0,1,0),
				glm::vec3(0,1,0),

				glm::vec3(0,0,1),
				glm::vec3(0,0,1),
			};


			std::vector<uint32_t> indices = {
				0,1,
				2,3,
				4,5
			};

			std::unique_ptr<Mesh> axisMesh = std::make_unique<Mesh>();
			axisMesh->setRenderType(GL_LINES);
			axisMesh->upload(vertices, {}, colors, indices);
			return axisMesh;
		}

		void Mesh::initBuffers() {
			glGenVertexArrays(1, &vao);
			if(vertices.size()) glGenBuffers(1, &vboPositions);
			if(normals.size()) glGenBuffers(1, &vboNormals);
			if(colors.size()) glGenBuffers(1, &vboColors);
			if(uvs2D.size() || uvs3D.size()) glGenBuffers(1, &vboUVs);
			if(indices.size()) glGenBuffers(1, &ibo);
		}
	}
}
