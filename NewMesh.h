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
			~Mesh() { unload(); }

			void setVertices(std::vector<glm::vec3> newVertices) { vertices = std::move(newVertices); }
			void setNormals(std::vector<glm::vec3> newNormals)   { normals = std::move(newNormals); }
			void setUv3Ds(std::vector<glm::vec3> newUv3D) { uvs3D = std::move(newUv3D); }
			void setUv2Ds(std::vector<glm::vec2> newUv2D) { uvs2D = std::move(newUv2D); }
			void setIndices(std::vector<uint32_t> newIndices) { indices = std::move(newIndices); }

			const std::vector<glm::vec3>& getVertices() const { return vertices; }
			const std::vector<glm::vec3>& getNormals() const { return normals; }
			const std::vector<glm::vec3>& getUv3Ds() const { return uvs3D; }
			const std::vector<glm::vec2>& getUv2Ds() const { return uvs2D; }
			const std::vector<uint32_t>& getIndices() const { return indices; }

			UVType getUVType() const { return uvType; }
			void setUVType(UVType uvType) {
				this->uvType = uvType;
			}

			void upload();
			void upload(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec3> uv3D, std::vector<uint32_t> indices);
			void upload(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uv2D, std::vector<uint32_t> indices);

			void unload();

			void bind();
			void render();

			static std::unique_ptr<Mesh> makeCube();

		private:
			void initBuffers();

			GLuint vao = 0;
			GLuint vboPositions = 0;
			GLuint vboNormals = 0;
			GLuint vboUVs = 0;
			GLuint ibo = 0;

			GLsizei indexCount = 0;

			UVType uvType = UVType::Vec2;

			std::vector<glm::vec3> vertices;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec3> uvs3D;
			std::vector<glm::vec2> uvs2D;
			std::vector<uint32_t> indices;
		};
	}
}