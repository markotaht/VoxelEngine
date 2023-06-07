#pragma once
#include "ShaderProgram.h"
#include "GenericShaderProgram.h"
#include "Mesh.h"
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include "Material.h"
class MeshRenderer
{
	Mesh* mesh;
	ShaderProgram* shaderProgram;
	Material* material;
	GLuint vao;

	void init();

	GLenum renderMode = GL_TRIANGLES;
public:
	MeshRenderer(Mesh* mesh, ShaderProgram* shaderProgram);
	MeshRenderer(Mesh* mesh, ShaderProgram* shaderProgram, Material* material);
	~MeshRenderer();

	void render();

	void bind() {
		if (material != nullptr) {
			material->setTextures();
		}
		shaderProgram->bind();
		if (material != nullptr) {
			material->setUniforms();
		}
	}
	void unbind() { shaderProgram->unbind(); }

	inline void setRenderMode(GLenum) { this->renderMode = renderMode; }

	ShaderProgram* getShader() { return shaderProgram; }
};

