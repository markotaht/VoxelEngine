#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <gl/glew.h>
#include <vector>
#include "Mesh.h"
#include <stdio.h>
#include "Loader.h"


class ResourceManager;

class OBJParser : public Loader<Mesh>
{
private:
	ResourceManager* manager;
public:
	OBJParser() {}
	OBJParser(ResourceManager* manager) {
		this->manager = manager;
	}
	Mesh* loadResource(const char* path);
};

