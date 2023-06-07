#ifndef BLOCK_H
#define BLOCK_H

enum class BlockType {
	BlockType_Default = 0,
};

class Block {
public:
	Block();

	bool IsActive() { return active; }

	static constexpr float BLOCK_SIZE = 0.01f;
private:
	bool active;
	BlockType blockType;

};

#endif // !BLOCK_H