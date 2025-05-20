#include "FontLoader.h"
#include "StringUtils.h"
#include "FileDescriptor.h"
#include "Font.h"

#include <memory>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <unordered_map>
#include <gl/glew.h>
#include <glm/glm.hpp>

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "AutoLoaderRegistrar.h"
#include "ResourceManager.h"

void dumpTexture(GLuint texture, int width, int height, const std::string& filename)
{
	std::vector<unsigned char> pixels(width * height);

	glBindTexture(GL_TEXTURE_2D, texture);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, pixels.data());

	// Optional: Flip vertically for human-friendly image (because OpenGL is bottom-up)
	std::vector<unsigned char> flipped(width * height);
	for (int row = 0; row < height; ++row) {
		memcpy(
			&flipped[row * width],
			&pixels[(height - 1 - row) * width],
			width
		);
	}

	stbi_write_png(filename.c_str(), width, height, 1, flipped.data(), width);
	std::cout << "Texture dumped to " << filename << std::endl;
}

namespace engine::loader {
	static AutoLoaderRegistrar<engine::loader::FontLoader, IResourceLoader<descriptor::FileDescriptor, asset::Font>> regTex([](resource::ResourceManager&) {
		return std::make_unique<engine::loader::FontLoader>();
		});

    bool FontLoader::canLoad(const descriptor::FileDescriptor& descriptor) const
    {
        return ends_with(descriptor.filePath, ".ttf");
    }

    std::unique_ptr<asset::Font> FontLoader::load(const descriptor::FileDescriptor& descriptor) const
    {
		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			std::cout << "Error:Freetype: Could not init FreeType Library" << std::endl;
			return nullptr;
		}

		FT_Face face;
		if (FT_New_Face(ft, descriptor.filePath.c_str(), 0, &face)) {
			std::cout << "ERROR:FREETYPE:Failed to load font: " << descriptor.filePath << std::endl;
			return nullptr;
		}

		FT_Set_Pixel_Sizes(face, 0, 48);

		const int NUM_CHARS = 128;
		stbrp_rect rects[NUM_CHARS];

		int padding = 1;
		int atlasWidth = 512, atlasHeight = 512;

		for (int c = 0; c < NUM_CHARS; c++) {
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;
			rects[c].id = c;
			rects[c].w = face->glyph->bitmap.width + padding;
			rects[c].h = face->glyph->bitmap.rows + padding;
		}

		stbrp_node nodes[512];
		stbrp_context context;
		stbrp_init_target(&context, atlasWidth, atlasHeight, nodes, 512);
		stbrp_pack_rects(&context, rects, NUM_CHARS);

		std::vector<unsigned char> atlasPixels(atlasWidth * atlasHeight, 0);
		std::unordered_map<char, asset::Character> characters;

		for (int i = 0; i < NUM_CHARS; i++) {
			if (!rects[i].was_packed) continue;

			char ch = (char)rects[i].id;
			if (FT_Load_Char(face, ch, FT_LOAD_RENDER)) continue;

			int x = rects[i].x;
			int y = rects[i].y;
			FT_Bitmap& bmp = face->glyph->bitmap;

			// Copy glyph bitmap to atlas
			for (unsigned int row = 0; row < bmp.rows; ++row) {
				for (unsigned int col = 0; col < bmp.width; ++col) {
					//int flippedRow = bmp.rows - 1 - row;
					//atlasPixels[(x + col) + (y + flippedRow) * atlasWidth] = bmp.buffer[col + row * bmp.width];
					atlasPixels[(x + col) + (y + row) * atlasWidth] = bmp.buffer[col + row * bmp.width];
				}
			}

			asset::Character character;
			character.uvTopLeft = glm::vec2(
				(float)x / atlasWidth,
				(float)(y + bmp.rows) / atlasHeight
			);

			character.uvBottomRight = glm::vec2(
				(float)(x + bmp.width) / atlasWidth,
				(float)(y) / atlasHeight
			);
			character.size = glm::ivec2(bmp.width, bmp.rows);
			character.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
			character.advance = face->glyph->advance.x;
			characters[ch] = character;
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		return std::make_unique<asset::Font>(descriptor.filePath, atlasWidth, atlasHeight, std::move(atlasPixels), std::move(characters));
    }

	bool FontLoader::uploadGPU(asset::Font& font) const
	{
		if (font.texture != 0) {
			glDeleteTextures(1, &font.texture);
		}
		glGenTextures(1, &font.texture);

		glBindTexture(GL_TEXTURE_2D, font.texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, font.atlasWidth, font.atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, font.atlasPixels.data());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);


		if (font.vao != 0) glDeleteVertexArrays(1, &font.vao);
		if (font.vbo != 0) glDeleteBuffers(1, &font.vbo);
		glGenVertexArrays(1, &font.vao);
		glGenBuffers(1, &font.vbo);
		glBindVertexArray(font.vao);
		glBindBuffer(GL_ARRAY_BUFFER, font.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//dumpTexture(texture, 512, 512, "font_atlas2.png");
		return true;
	}

}
