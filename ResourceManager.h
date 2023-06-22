#pragma once
#include <map>
#include <atomic>
#include <string>
#include "Resource.h"

#include "ImageLoader.h"
#include "OBJParser.h"
#include "Loader.h"
#include "MTLParser.h"

#include <utility>

class ResourceManager
{
	std::map<std::string, Resource*>  resources;
	std::atomic_uint32_t currentId = 0;
public:

	std::pair < std::string, std::vector<std::pair<std::string, std::string>>> model;
	template <typename T>
	uint32_t AddResource(std::string filePath) {

		auto it = resources.find(filePath);
		if (it != resources.end())
		{
			return it->second->getId();
		}

		Loader<T>* loader = getLoader<T>();
		if (loader == nullptr) {
			return -1;
		}
		Resource* resource = loader->loadResource(filePath.c_str());
		if (resource == nullptr) {
			return -1;
		}
		resource->setId(currentId++);
		resources[filePath] = resource;
		return resource->getId();
	}

	template <typename T>
	uint32_t AddResource(std::string filePath, std::string filePath2) {

		auto it = resources.find(filePath);
		if (it != resources.end())
		{
			return it->second->getId();
		}

		Resource* resource = new T();
		if (!resource->loadFromFile(filePath.c_str(), filePath2.c_str())) {
			return -1;
		}
		resource->setId(currentId++);
		resources[filePath] = resource;
		return resource->getId();
	}

	template <typename T>
	uint32_t AddResource(std::string name, T* resource) {

		auto it = resources.find(name);
		if (it != resources.end())
		{
			return it->second->getId();
		}

		resource->setId(currentId++);
		resources[name] = resource;
		return resource->getId();
	}

	template<typename T>
	T* getResource(uint32_t id) {
		auto it = resources.begin();
		while (it != resources.end()) {
			if (it->second->getId() == id) {
				return static_cast<T*>(it->second);
			}
			it++;
		}

		return NULL;
	}

	template<typename T>
	T* getResource(std::string id) {
		return static_cast<T*>(resources[id]);
	}

	template<typename T>
	Loader<T>* getLoader() {
		return nullptr;
	}

	template<>
	Loader<Texture>* getLoader<Texture>() {
		return new ImageLoader();
	}

	template<>
	Loader<Mesh>* getLoader<Mesh>() {
		return new OBJParser(this);
	}

	template<>
	Loader<MTLMaterial>* getLoader<MTLMaterial>() {
		return new MTLParser(this);
	}

	/*uint32_t getCubeMesh() {
		auto it = resources.find("IN-BUILT-CUBE");
		if (it != resources.end())
		{
			return it->second->getId();
		}

		Mesh* cubeMesh = Mesh::cubePrimitive();
		cubeMesh->setId(currentId++);
		resources["IN-BUILT-CUBE"] = cubeMesh;
		return cubeMesh->getId();
	}

	uint32_t getPlaneMesh() {
		auto it = resources.find("IN-BUILT-PLANE");
		if (it != resources.end())
		{
			return it->second->getId();
		}

		Mesh* cubeMesh = Mesh::planePrimitive();
		cubeMesh->setId(currentId++);
		resources["IN-BUILT-PLANE"] = cubeMesh;
		return cubeMesh->getId();
	}*/
};

