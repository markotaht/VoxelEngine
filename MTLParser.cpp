#include "MTLParser.h"
#include "ResourceManager.h"
float naive_atof2(const char* p) {
	float r = 0.0;
	bool neg = false;
	if (*p == '-') {
		neg = true;
		++p;
	}
	while (*p >= '0' && *p <= '9') {
		r = (r * 10.0f) + (*p - '0');
		++p;
	}
	if (*p == '.') {
		float f = 0.0;
		int n = 0;
		++p;
		while (*p >= '0' && *p <= '9') {
			f = (f * 10.0f) + (*p - '0');
			++p;
			++n;
		}
		r += f / std::pow(10.0f, n);
	}
	if (neg) {
		r = -r;
	}
	return r;
}
MTLMaterial* MTLParser::loadResource(const char* path)
{
	FILE* fp;
	errno_t err = fopen_s(&fp, path, "r");
	char line[256] = { 0 };
	char type[256] = { 0 };
	char xyz[3][100] = { 0 };

	std::vector<MTLMaterial*> materials;

	int materialCount = -1;
	while (fgets(line, sizeof(line), fp)) {
		if (line[0] == '#') continue;
		GLfloat x, y, z;
		sscanf_s(line, "%s %s %s %s", type, _countof(type), xyz[0], _countof(xyz[0]), xyz[1], _countof(xyz[1]), xyz[2], _countof(xyz[2]));
		if (strcmp(type, "newmtl") == 0) {
			materialCount++;
			materials.push_back(new MTLMaterial());
			manager->AddResource(xyz[0], materials[materialCount]);
			continue;
		}
		x = naive_atof2(xyz[0]);
		y = naive_atof2(xyz[1]);
		z = naive_atof2(xyz[2]);

		if (strcmp(type, "Ka") == 0) {
			materials[materialCount]->setAmbientColor(glm::vec3(x, y, z));
		}
		else if (strcmp(type, "Kd") == 0) {
			materials[materialCount]->setDiffuseColor(glm::vec3(x, y, z));
		}
		else if (strcmp(type, "Ks") == 0) {
			materials[materialCount]->setSpecularColor(glm::vec3(x, y, z));
		}
		else if (strcmp(type, "Ke") == 0) {
			materials[materialCount]->setEmissiveColor(glm::vec3(x, y, z));
		}
		else if (strcmp(type, "Ns") == 0) {
			materials[materialCount]->setShininess(x);
		}
		else if (strcmp(type, "d") == 0) {
			materials[materialCount]->setOpacity(x);
		}
	}

	return materials[0];
}
