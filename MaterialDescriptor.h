#pragma once
#include <string>
#include <ostream>
#include <unordered_map>
#include <variant>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include "ShaderDescriptor.h"
#include "FileDescriptor.h"
#include "Texture2DArrayDescriptor.h"
#include "Types.h"

namespace engine {
	namespace descriptor {
		using TextureDescriptorVariant = std::variant<
			descriptor::FileDescriptor,
			descriptor::Texture2DArrayDescriptor
		>;

		struct MaterialDescriptor {
			ShaderDescriptor shaderDescriptor;
			std::unordered_map<std::string, UniformValue> uniforms;
			std::unordered_map<std::string, TextureDescriptorVariant> textures;

			bool operator==(const MaterialDescriptor& other) const {
				return shaderDescriptor == other.shaderDescriptor && uniforms == other.uniforms
					&& textures == other.textures;
			}
		};
	}
}

inline std::ostream& operator<<(std::ostream& os, const engine::UniformValue& v) {
	std::visit([&os](auto&& val) {
		os << val;
		}, v);
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const std::unordered_map<std::string, engine::UniformValue>& uniforms) {
	os << "{ ";
	for (const auto& [k, v] : uniforms) {
		os << k << ": " << v << ", ";
	}
	os << "}";
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const engine::descriptor::MaterialDescriptor& desc) {
	os << desc.shaderDescriptor << ", " << desc.uniforms;
	return os;
}

namespace std {
	template<>
	struct hash<engine::UniformValue> {
		std::size_t operator()(const engine::UniformValue& v) const {
			return std::visit([](auto&& arg) {
				return std::hash<std::decay_t<decltype(arg)>>{}(arg);
				}, v);
		}
	};

	inline std::size_t hashUniforms(const std::unordered_map<std::string, engine::UniformValue>& uniforms) {
		std::size_t result = 0;
		for (const auto& [key, value] : uniforms) {
			std::size_t h1 = std::hash<std::string>{}(key);
			std::size_t h2 = std::hash<engine::UniformValue>{}(value);
			result ^= (h1 ^ (h2 << 1));
		}
		return result;
	}

	inline std::size_t hashTextures(const std::unordered_map<std::string, engine::descriptor::TextureDescriptorVariant>& textures) {
		std::size_t result = 0;
		for (const auto& [key, value] : textures) {
			std::size_t h1 = std::hash<std::string>{}(key);
			std::size_t h2 = std::hash<engine::descriptor::TextureDescriptorVariant>{}(value);
			result ^= (h1 ^ (h2 << 1));
		}
		return result;
	}

	template<>
	struct hash<engine::descriptor::MaterialDescriptor> {
		std::size_t operator()(const engine::descriptor::MaterialDescriptor& desc) const {
			return std::hash<engine::descriptor::ShaderDescriptor>{}(desc.shaderDescriptor)
				^ (hashUniforms(desc.uniforms) << 1)
				^ (hashTextures(desc.textures) << 2);
		}
	};
}