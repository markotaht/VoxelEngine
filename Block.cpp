#include "Block.h"

std::unordered_map<BlockType, BlockTextureData*> BlockTextures = {
	{BlockType::BlockType_Default, new BlockTextureData{"UV","UV","UV","UV","UV", "UV"}},
	{BlockType::BlockType_Grass, new BlockTextureData{"GrassTop","GrassBottom","GrassSide","GrassSide","GrassSide", "GrassSide"}}
};

std::array < BlockTextureData, static_cast<size_t>(BlockType::BlockCount)> blockTextures = []() {
	std::array < BlockTextureData, static_cast<size_t>(BlockType::BlockCount)> arr{};
	arr[static_cast<size_t>(BlockType::BlockType_Default)] = { "UV","UV","UV","UV","UV", "UV" };
	arr[static_cast<size_t>(BlockType::BlockType_Grass)] = { "GrassTop","GrassBottom","GrassSide","GrassSide","GrassSide", "GrassSide" };
	return arr;
}();