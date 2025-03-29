#pragma once
#include "ShaderDescriptor.h"
#include "FileDescriptor.h"
#include <ostream>

namespace engine {
	namespace descriptor {

		struct TextMaterialDescriptor {
			ShaderDescriptor shaderDescriptor;
			FileDescriptor fontDescriptor;

			bool operator==(const TextMaterialDescriptor& other) const {
				return shaderDescriptor == other.shaderDescriptor && fontDescriptor == other.fontDescriptor;
			}
		};
	}
}

inline std::ostream& operator<<(std::ostream& os, const engine::descriptor::TextMaterialDescriptor& desc) {
	os << desc.shaderDescriptor << ", " << desc.fontDescriptor;
	return os;
}

namespace std {
	template<>
	struct hash<engine::descriptor::TextMaterialDescriptor> {
		std::size_t operator()(const engine::descriptor::TextMaterialDescriptor& desc) const {
			return std::hash<engine::descriptor::ShaderDescriptor>{}(desc.shaderDescriptor)
				^ (std::hash<engine::descriptor::FileDescriptor>{}(desc.fontDescriptor) << 1);
		}
	};
}