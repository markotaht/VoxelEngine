#pragma once
#include <vector>
#include <utility>
#include <glm/glm.hpp>
#include <gl/glew.h>
#include <memory>


#include "NewResource.h"

namespace engine {
	namespace asset {
		enum class UVType { Vec2, Vec3 };

		class Mesh : public resource::Resource
		{
		public:
			Mesh() = default;
			Mesh(Mesh&& other) noexcept {
				*this = std::move(other);
			}
			Mesh& operator=(Mesh&& other) noexcept {
				if (this != &other) {
					// First unload existing resources if any
					unload();

					// Move GPU handles
					vao = other.vao; other.vao = 0;
					vboPositions = other.vboPositions; other.vboPositions = 0;
					vboNormals = other.vboNormals; other.vboNormals = 0;
					vboColors = other.vboColors; other.vboColors = 0;
					vboUVs = other.vboUVs; other.vboUVs = 0;
					ibo = other.ibo; other.ibo = 0;

					// Move render state
					indexCount = other.indexCount;
					renderType = other.renderType;
					uvType = other.uvType;

					// Move data vectors
					vertices = std::move(other.vertices);
					normals = std::move(other.normals);
					colors = std::move(other.colors);
					uvs3D = std::move(other.uvs3D);
					uvs2D = std::move(other.uvs2D);
					indices = std::move(other.indices);
				}
				return *this;
			}
			~Mesh() { unload(); }

			void setVertices(std::vector<glm::vec3> newVertices) { vertices = std::move(newVertices); }
			void setNormals(std::vector<glm::vec3> newNormals)   { normals = std::move(newNormals); }
			void setColors(std::vector<glm::vec3> newColors)   { colors = std::move(newColors); }
			void setUv3Ds(std::vector<glm::vec3> newUv3D) { uvs3D = std::move(newUv3D); }
			void setUv2Ds(std::vector<glm::vec2> newUv2D) { uvs2D = std::move(newUv2D); }
			void setIndices(std::vector<uint32_t> newIndices) { indices = std::move(newIndices); }
			void setRenderType(GLenum newRenderType) { renderType = newRenderType; }

			const std::vector<glm::vec3>& getVertices() const { return vertices; }
			const std::vector<glm::vec3>& getNormals() const { return normals; }
			const std::vector<glm::vec3>& getColors() const { return colors; }
			const std::vector<glm::vec3>& getUv3Ds() const { return uvs3D; }
			const std::vector<glm::vec2>& getUv2Ds() const { return uvs2D; }
			const std::vector<uint32_t>& getIndices() const { return indices; }

			UVType getUVType() const { return uvType; }
			void setUVType(UVType uvType) {
				this->uvType = uvType;
			}

			void upload();
			void upload(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec3> colors, std::vector<glm::vec3> uv3D, std::vector<uint32_t> indices);
			void upload(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec3> colors, std::vector<glm::vec2> uv2D, std::vector<uint32_t> indices);
			void upload(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec3> colors, std::vector<uint32_t> indices);

			void unload();

			void bind();
			void render();

			static std::unique_ptr<Mesh> makeCube();
			static std::unique_ptr<Mesh> makeAxis();

		private:
			void initBuffers();

			GLuint vao = 0;
			GLuint vboPositions = 0;
			GLuint vboNormals = 0;
			GLuint vboColors = 0;
			GLuint vboUVs = 0;
			GLuint ibo = 0;

			GLsizei indexCount = 0;

			UVType uvType = UVType::Vec2;
			GLenum renderType = GL_TRIANGLES;

			std::vector<glm::vec3> vertices;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec3> colors;
			std::vector<glm::vec3> uvs3D;
			std::vector<glm::vec2> uvs2D;
			std::vector<uint32_t> indices;
		};
	}
}