#pragma once
#include "ShaderProgram.h"
#include "GenericShaderProgram.h"
#include "Mesh.h"
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include "Material.h"
#include <iostream>
class MeshRenderer
{
	Mesh* mesh;
	Material* material;
	GLuint vao;

	void init();

	GLenum renderMode = GL_TRIANGLES;
public:
	MeshRenderer(Mesh* mesh, Material* material);
	~MeshRenderer();

	void render();

	void bind() {
		if (material == nullptr) {
			std::cout << "Object material is missing. Cannot render object." << std::endl;
		}
		material->setTextures();
		material->getShaderProgram()->bind();
		material->setUniforms();
	}
	void unbind() { material->getShaderProgram()->unbind(); }

	inline void setRenderMode(GLenum newRendermode) { this->renderMode = newRendermode; }

	inline ShaderProgram* getShader() { return material->getShaderProgram(); }
};

