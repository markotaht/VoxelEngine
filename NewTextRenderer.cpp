#include "TextRenderer.h"
#include "TextMaterial.h"
#include "Window.h"
#include "Font.h"

#include <string>

void engine::render::TextRenderer::render(resource::ResourceManager& resourceManager)
{
	asset::TextMaterial* textMaterial = resourceManager.get<asset::TextMaterial>(textMaterialId);
    asset::Font* font = resourceManager.get<asset::Font>(textMaterial->getFontId());
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Window::SCREEN_WIDTH), 0.0f, static_cast<float>(Window::SCREEN_HEIGHT));
	textMaterial->bind(glm::vec3(1.f, 1.f, 1.f), projection, resourceManager);
    for (size_t i = 0; i < messages.size(); i++) {
		_render(messages[i], 0, 24.0f * i, 0.5f, font);
	}
	textMaterial->unbind(resourceManager);
    messages.clear();
}

void engine::render::TextRenderer::_render(std::string& text, float x, float y, float scale, asset::Font* font)
{
    for (char c : text) {
        asset::Character ch = font->getCharater(c);

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float u0 = ch.uvTopLeft.x;
        float v0 = ch.uvTopLeft.y;
        float u1 = ch.uvBottomRight.x;
        float v1 = ch.uvBottomRight.y;

        float vertices[6][4] = {
            { xpos,     ypos + h,   u0, v1 },
            { xpos,     ypos,       u0, v0 },
            { xpos + w, ypos,       u1, v0 },

            { xpos,     ypos + h,   u0, v1 },
            { xpos + w, ypos,       u1, v0 },
            { xpos + w, ypos + h,   u1, v1 }
        };

        glBindBuffer(GL_ARRAY_BUFFER, font->getVbo());
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.advance >> 6) * scale;
    }

}
