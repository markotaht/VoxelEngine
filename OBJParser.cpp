#include "OBJParser.h"
#include <chrono>
#include <iostream>
#include "MTLParser.h"
#include "ResourceManager.h"
#include <string>
#include <utility>
#include <vector>

float naive_atof(const char* p) {
	float r = 0.0;
	bool neg = false;
	if (*p == '-') {
		neg = true;
		++p;
	}
	while (*p >= '0' && *p <= '9') {
		r = (r * 10.0) + (*p - '0');
		++p;
	}
	if (*p == '.') {
		float f = 0.0;
		int n = 0;
		++p;
		while (*p >= '0' && *p <= '9') {
			f = (f * 10.0) + (*p - '0');
			++p;
			++n;
		}
		r += f / std::pow(10.0, n);
	}
	if (neg) {
		r = -r;
	}
	return r;
}

int convertToInt(char a[1000]) {
	int i = 0;
	int num = 0;
	while (a[i] != 0)
	{
		num = (a[i] - '0') + (num * 10);
		i++;
	}
	return num;;
}

Mesh* OBJParser::loadResource(const char* path)
{

	FILE* fp;
	errno_t err = fopen_s(&fp, path, "r");
	char line[256] = { 0 };
	char type[256] = { 0 };
	char meshName[256] = { 0 };
	char materialName[256] = { 0 };
	char xyz[3][100] = { 0 };

	size_t verticesCount = 0;
	size_t normalsCount = 0;
	size_t uvCount = 0;
	size_t faceCount = 0;

	std::vector<std::pair<std::string, std::string>> meshMaterial;

	while (fgets(line, sizeof(line), fp)) {
		if (line[0] == 'v') {
			if (line[1] == ' ') {
				++verticesCount;
			}
			else if (line[1] == 'n') {
				++normalsCount;
			}
			else if (line[1] == 't') {
				++uvCount;
			}
		}
		else if (line[0] == 'f') {
			++faceCount;
		}
	}
	fopen_s(&fp, path, "r");

	std::vector<GLfloat> vertices(verticesCount * 3);
	std::vector<GLfloat> normals(normalsCount * 3);
	std::vector<GLfloat> UVs(uvCount * 3);

	std::vector<GLfloat> out_vertices(faceCount * 9 * 2);
	std::vector<GLfloat> out_normals(faceCount * 9 * 2);
	std::vector<GLfloat> out_UVs(faceCount * 6 * 2);
	std::vector<GLuint> out_indices;

	std::chrono::steady_clock::time_point getLineTime;

	auto fullParsingStart = std::chrono::high_resolution_clock::now();
	size_t vertex = 0;
	size_t normal = 0;
	size_t uv = 0;

	size_t vertex2 = 0;
	size_t normal2 = 0;
	size_t uv2 = 0;
	size_t index = 0;

	std::vector<Mesh*> meshes;

	int oCOunt = 0;
	int partCount = 0;
	while (fgets(line, sizeof(line), fp)) {
		if (line[0] == 'v') {
			GLfloat x, y, z;
			sscanf_s(line, "%s %s %s %s", type, _countof(type), xyz[0], _countof(xyz[0]), xyz[1], _countof(xyz[1]), xyz[2], _countof(xyz[2]));
			x = naive_atof(xyz[0]);
			y = naive_atof(xyz[1]);
			z = naive_atof(xyz[2]);
			if (strcmp(type, "v") == 0) {
				vertices[vertex] = x;
				vertices[vertex + 1] = y;
				vertices[vertex + 2] = z;
				vertex += 3;
			}
			else if (strcmp(type, "vn") == 0) {
				normals[normal] = x;
				normals[normal + 1] = y;
				normals[normal + 2] = z;
				normal += 3;
			}
			else if (strcmp(type, "vt") == 0) {
				UVs[uv] = x;
				UVs[uv + 1] = y;
				uv += 2;
			}
		}
		else if (line[0] == 'f') {
			bool vert = false, uv = false, norm = false;
			char vertIc[5][100] = { 0 };
			char uvIc[5][100] = { 0 };
			char normIc[5][100] = { 0 };
			int verti = 0, uvI = 0, normI = 0;
			int verti2 = 0, uvI2 = 0, normI2 = 0;
			for (int i = 1; i < 256; i++) {
				if (line[i] == '\n') break;
				else if (line[i] == ' ') {

					if (norm) {
						normI++;
						normI2 = 0;
					}
					uv = norm = false;
					vert = true;
				}
				else if (line[i] == '/') {
					if (vert) {
						verti++;
						verti2 = 0;
					}
					else if (uv) {
						uvI++;
						uvI2 = 0;
					}
					norm = uv;
					uv = vert;
					vert = false;
				}
				else {
					if (vert) {
						vertIc[verti][verti2++] = line[i];
					}
					else if (uv) {
						uvIc[uvI][uvI2++] = line[i];
					}
					else if (norm) {
						normIc[normI][normI2++] = line[i];
					}
				}
			}

			unsigned int vertCount = vertex2;
			unsigned int uvCount = uv2;
			unsigned int normCount = normal2;
			for (int i = 0; i < verti; i++) {
				int vertIndex = convertToInt(vertIc[i]) - 1;

				out_vertices[vertCount + i * 3] = vertices[vertIndex * 3];
				out_vertices[vertCount + i * 3 + 1] = vertices[vertIndex * 3 + 1];
				out_vertices[vertCount + i * 3 + 2] = vertices[vertIndex * 3 + 2];


				if (UVs.size() > 0) {
					int uvIdx = convertToInt(uvIc[i]) - 1;
					out_UVs[uvCount + i * 2] = UVs[uvIdx * 2];
					out_UVs[uvCount + i * 2 + 1] = UVs[uvIdx * 2 + 1];
				}

				int normIdx = convertToInt(normIc[i]) - 1;
				out_normals[normCount + i * 3] = normals[normIdx * 3];
				out_normals[normCount + i * 3 + 1] = normals[normIdx * 3 + 1];
				out_normals[normCount + i * 3 + 2] = normals[normIdx * 3 + 2];
			}

			vertex2 += verti * 3;
			uv2 += verti * 2;
			normal2 += verti * 3;

			if (verti == 3) {
				out_indices.push_back(index + 0);
				out_indices.push_back(index + 1);
				out_indices.push_back(index + 2);
			}
			else if (verti == 4) {
				out_indices.push_back(index + 0);
				out_indices.push_back(index + 1);
				out_indices.push_back(index + 3);

				out_indices.push_back(index + 1);
				out_indices.push_back(index + 2);
				out_indices.push_back(index + 3);
			}
			index += verti;
		}
		else if (line[0] == 'o') {
			if (out_indices.size() > 0) {
				meshes.push_back(new Mesh(&out_vertices[0], &out_normals[0], &out_UVs[0], &out_indices[0], out_vertices.size(), out_normals.size(), out_UVs.size(), out_indices.size()));
				manager->AddResource(std::string(meshName) + "." + std::string(materialName), meshes[meshes.size() - 1]);
				out_normals.clear();
				out_UVs.clear();
				out_indices.clear();

				out_normals.resize(faceCount * 9 * 2);
				out_UVs.resize(faceCount * 6 * 2);
				vertex2 = 0;
				uv2 = 0;
				normal2 = 0;
				index = 0;
			}
			sscanf_s(line, "o %s", &meshName, 256);
			out_vertices.clear();
			out_vertices.resize(faceCount * 9 * 2);
			oCOunt++;
		}
		else if (line[0] == 'u') {
			if (out_indices.size() > 0) {
				meshes.push_back(new Mesh(&out_vertices[0], &out_normals[0], &out_UVs[0], &out_indices[0], out_vertices.size(), out_normals.size(), out_UVs.size(), out_indices.size()));
				manager->AddResource(std::string(meshName) + "." + std::string(materialName), meshes[meshes.size() - 1]);
				out_normals.clear();
				out_UVs.clear();
				out_indices.clear();

				out_normals.resize(faceCount * 9 * 2);
				out_UVs.resize(faceCount * 6 * 2);
				vertex2 = 0;
				uv2 = 0;
				normal2 = 0;
				index = 0;
			}
			sscanf_s(line, "usemtl %s", &materialName, 256);
			meshMaterial.push_back(std::make_pair(std::string(meshName) + "." + std::string(materialName), std::string(materialName)));
		}
	}
	meshes.push_back(new Mesh(&out_vertices[0], &out_normals[0], &out_UVs[0], &out_indices[0], out_vertices.size(), out_normals.size(), out_UVs.size(), out_indices.size()));
	manager->AddResource(std::string(meshName) + "." + std::string(materialName), meshes[meshes.size() - 1]);
	manager->model = std::make_pair(path, meshMaterial);
	auto fullParsingEnd = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(fullParsingEnd - fullParsingStart).count();
	//	std::cout << duration << std::endl;
	return meshes[0];
}
