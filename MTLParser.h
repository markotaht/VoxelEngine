#pragma once
#include "Loader.h"
#include "MTLMaterial.h"

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string>
#include <gl/glew.h>
#include <vector>

#include "GenericShaderProgram.h"
class ResourceManager;
class MTLParser : public Loader<MTLMaterial>
{
	ResourceManager* manager;
public:
	MTLParser() {}
	MTLParser(ResourceManager* manager) {
		this->manager = manager;
	}
	MTLMaterial* loadResource(const char* path);
};

