#pragma once

#include <unordered_map>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>

#include <string>
struct ChunkData {
	std::string elements;
	glm::ivec3 chunkNumber;
	int maxHeight;
	bool loaded;
};

class ChunkStorage
{
public:
	ChunkStorage() {};

	inline bool hasChunk(glm::ivec3 chunkNumber) {
		return storedChunks.find(chunkNumber) != storedChunks.end();
	}
	inline void addChunk(ChunkData* data, glm::ivec3 pos) {
		storedChunks[pos] = data;
	}

	inline ChunkData* getChunk(glm::ivec3 chunkNumber) {
		return storedChunks[chunkNumber];
	}

private:
	std::unordered_map<glm::ivec3, ChunkData*, std::hash<glm::ivec3> > storedChunks;
};

