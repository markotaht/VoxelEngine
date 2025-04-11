#include "NewMaterial.h"
#include "ITexture.h"
#include "NewShaderProgram.h"
#include "NewResourceManager.h"

#include <gl/glew.h>

namespace engine {
	namespace asset {
		void Material::bind(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, resource::ResourceManager& resourceManager)
		{
			ShaderProgram* shaderProgram = resourceManager.get<ShaderProgram>(shaderProgramId);
			if (!shaderProgram) return;
			shaderProgram->bind();

            GLint modelLoc = glGetUniformLocation(shaderProgram->getProgramId(), "model");
            GLint viewLoc = glGetUniformLocation(shaderProgram->getProgramId(), "view");
            GLint projLoc = glGetUniformLocation(shaderProgram->getProgramId(), "projection");

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


            bindUniforms(shaderProgram);
            bindTextures(shaderProgram, resourceManager);
		}

		void Material::bindUniforms(ShaderProgram* shaderProgram)
		{
            for (const auto& [name, value] : uniforms) {
                GLint location = shaderProgram->getUniform(name.c_str());
                if (location == -1) continue; // Skip if not found in shader

                std::visit([&](auto&& v) {
                    using T = std::decay_t<decltype(v)>;

                    if constexpr (std::is_same_v<T, int>) {
                        glUniform1i(location, v);
                    }
                    else if constexpr (std::is_same_v<T, float>) {
                        glUniform1f(location, v);
                    }
                    else if constexpr (std::is_same_v<T, glm::vec2>) {
                        glUniform2fv(location, 1, &v[0]);
                    }
                    else if constexpr (std::is_same_v<T, glm::vec3>) {
                        glUniform3fv(location, 1, &v[0]);
                    }
                    else if constexpr (std::is_same_v<T, glm::vec4>) {
                        glUniform4fv(location, 1, &v[0]);
                    }
                    else if constexpr (std::is_same_v<T, glm::mat4>) {
                        glUniformMatrix4fv(location, 1, GL_FALSE, &v[0][0]);
                    }
                    else {
                        std::cerr << "Material uniform '" << name << "' has unsupported type.\n";
                        static_assert(always_false<T>::value, "Unsupported uniform type!");
                    }

                    }, value);
            }
		}
        void Material::bindTextures(ShaderProgram* shaderProgram, resource::ResourceManager& resourceManager)
        {
			GLuint unit = 0;
			for (auto& [id, textureId] : textures) {
				ITexture* texture = resourceManager.get<ITexture>(textureId);
				if (!texture) continue;
                texture->bind(unit);

				GLint location = shaderProgram->getUniform(id.c_str());
				if (location != -1) {
					glUniform1i(location, unit);
				}

				++unit;
			}
        }
	}
}
