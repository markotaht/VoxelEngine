#pragma once
#include <cstdint>
class Resource
{
	uint32_t id;
public:
	uint32_t getId() { return id; }
	void setId(uint32_t id) { this->id = id; }

	virtual bool loadFromFile(const char* path) = 0;
	virtual bool loadFromFile(const char* path, const char* path2) = 0;
};

