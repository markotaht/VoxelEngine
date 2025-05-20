#pragma once
#include <vector>
#include <string>
#include "ResourceId.h"
#include "TextMaterial.h"
#include "ResourceManager.h"

namespace engine::render {
	class TextRenderer
	{
	public:
		TextRenderer(core::ResourceId<asset::TextMaterial> textMaterialId): textMaterialId(textMaterialId){}

		void render(resource::ResourceManager& resourceManager);

		inline void addMessage(std::string message) {
			messages.push_back(message);
		}
	private:
		void _render(std::string& text, float x, float y, float scale, asset::Font* font);

		std::vector<std::string> messages;
		core::ResourceId<asset::TextMaterial> textMaterialId;
	};
}

