#include "Block.h"

std::map<BlockType, BlockTextureData*> BlockTextures = {
	{BlockType::BlockType_Default, new BlockTextureData{"UV","UV","UV","UV","UV", "UV"}},
	{BlockType::BlockType_Grass, new BlockTextureData{"GrassTop","GrassBottom","GrassSide","GrassSide","GrassSide", "GrassSide"}}
};

Block::Block()
{
	active = true;
	blockType = BlockType::BlockType_Default;
}
