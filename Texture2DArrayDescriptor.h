#pragma once

#include <string>
#include <ostream>
#include <unordered_map>
#include "FileDescriptor.h"

namespace engine {
	namespace descriptor {
		struct Texture2DArrayDescriptor {
			int width;
			int height;
			std::unordered_map<std::string, FileDescriptor> textures;

			bool operator==(const Texture2DArrayDescriptor& other) const {
				return textures == other.textures && width == other.width && height == other.height;
			}
		};
	}
}

inline std::ostream& operator<<(std::ostream& os, const std::unordered_map<std::string, engine::descriptor::FileDescriptor>& descriptors) {
	os << "{ ";
	for (const auto& [k, v] : descriptors) {
		os << k << ": " << v << ", ";
	}
	os << "}";
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const engine::descriptor::Texture2DArrayDescriptor& desc) {
	os << "Width: " << desc.width << ", heigt: " << desc.height << ", textures: " << desc.textures;
	return os;
}

inline std::size_t hashTextures(const std::unordered_map<std::string, engine::descriptor::FileDescriptor>& textures) {
	std::size_t result = 0;
	for (const auto& [key, value] : textures) {
		std::size_t h1 = std::hash<std::string>{}(key);
		std::size_t h2 = std::hash<engine::descriptor::FileDescriptor>{}(value);
		result ^= (h1 ^ (h2 << 1));
	}
	return result;
}

namespace std {
	template<>
	struct hash<engine::descriptor::Texture2DArrayDescriptor> {
		std::size_t operator()(const engine::descriptor::Texture2DArrayDescriptor& desc) const {
			return hashTextures(desc.textures);
		}
	};
}