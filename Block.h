#ifndef BLOCK_H
#define BLOCK_H

#include <map>
#include <string>

enum class BlockType {
	BlockType_Default = 0,
	BlockType_Grass = 1,
};

struct BlockTextureData {
	std::string top;
	std::string bottom;
	std::string left;
	std::string right;
	std::string front;
	std::string back;
};

extern std::map<BlockType, BlockTextureData*> BlockTextures;

class Block {
public:
	Block();
	Block(BlockType blockType) :blockType{ blockType }, active{ true }{};

	inline bool IsActive() { return active; }
	inline bool setActive(bool active) { this->active = active; }
	inline BlockType getBlockType() { return blockType; }

	static constexpr float BLOCK_SIZE = 0.01f;
private:
	bool active;
	BlockType blockType;

};

#endif // !BLOCK_H