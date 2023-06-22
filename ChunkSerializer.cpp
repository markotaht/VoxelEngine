#include "ChunkSerializer.h"
#include "Chunk.h"
ChunkData* chunkSerializer::serialize(Chunk* chunk)
{
	std::string result = "";

	int count = 1;
	for (size_t i = 0; i < chunk->blocks.size(); i++) {
		if (i != chunk->blocks.size() -1 && chunk->blocks[i] == chunk->blocks[i + 1]) {
			count++;
		}
		else {
			result += std::to_string(count) + ":" + std::to_string(static_cast<int>(chunk->blocks[i])) + "!";
			count = 1;
		}
	}

	return new ChunkData{ result, chunk->chunkNumber, chunk->maxHeight, true };
}

Chunk* chunkSerializer::deserialize(ChunkData* data, Material* material)
{
	Chunk* chunk = new Chunk();
	chunk->chunkNumber = data->chunkNumber;
	chunk->maxHeight = data->maxHeight;
	chunk->material = material;

	std::string acc = "";
	int number = 0;
	int value = 0;
	bool addValues = false;

	int running_index = 0;
	for (size_t i = 0; i < data->elements.size(); i++) {
		if (data->elements[i] == ':') {
			number = std::stoi(acc);
			acc = "";
		}
		else if (data->elements[i] == '!') {
			value = std::stoi(acc);
			acc = "";
			addValues = true;
		}
		else {
			acc += data->elements[i];
		}

		if (addValues) {
			for (; number > 0; number--, running_index++) {
				chunk->blocks[running_index] = BlockType(value);
			}
			addValues = false;
		}
	}

	chunk->calculateBlocksToRender();
	return chunk;
}

