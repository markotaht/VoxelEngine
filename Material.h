#pragma once
#include "ShaderProgram.h"
class Material : public Resource {
protected:
	ShaderProgram* shaderProgram;
public:
	virtual void setUniforms() = 0;
	virtual void setTextures() = 0;
	bool loadFromFile(const char* path) { return false; }
	bool loadFromFile(const char* path, const char* path2) { return false; }
	inline void setShaderProgram(ShaderProgram* shaderProgram) { this->shaderProgram = shaderProgram; }
	inline ShaderProgram* getShaderProgram() { return shaderProgram; }
};