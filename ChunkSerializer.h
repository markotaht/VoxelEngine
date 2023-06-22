#pragma once

#include "Block.h"
#include "ChunkStorage.h"
#include "Material.h"

#include <string>


class Chunk;
namespace chunkSerializer {
	ChunkData* serialize(Chunk* chunk);
	Chunk* deserialize(ChunkData* data, Material* material);
}
