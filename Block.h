#ifndef BLOCK_H
#define BLOCK_H

#include <unordered_map>
#include <string>

#include <array>

enum class BlockType : uint8_t {
	BlockType_Air = 0,
	BlockType_Default = 1,
	BlockType_Grass = 2,
	BlockCount
};

struct BlockTextureData {
	std::string top;
	std::string bottom;
	std::string left;
	std::string right;
	std::string front;
	std::string back;
};

extern std::unordered_map<BlockType, BlockTextureData*> BlockTextures;
extern std::array < BlockTextureData, static_cast<size_t>(BlockType::BlockCount)> blockTextures;

static const float BLOCK_SIZE = 0.01f;
static const float BLOCK_WIDTH = 2 * BLOCK_SIZE;

#endif // !BLOCK_H