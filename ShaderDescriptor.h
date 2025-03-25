#pragma once
#include <string>
#include <ostream>

namespace engine {
	namespace descriptor {
		struct ShaderDescriptor {
			std::string vertexPath;
			std::string fragmentPath;

			bool operator==(const ShaderDescriptor& other) const {
				return vertexPath == other.vertexPath && fragmentPath == other.fragmentPath;
			}
		};
	}
}

inline std::ostream& operator<<(std::ostream& os, const engine::descriptor::ShaderDescriptor& desc) {
	os << desc.vertexPath << ", " << desc.fragmentPath;
	return os;
}

namespace std {
	template<>
	struct hash<engine::descriptor::ShaderDescriptor> {
		std::size_t operator()(const engine::descriptor::ShaderDescriptor& desc) const {
			return std::hash<std::string>{}(desc.vertexPath) ^
				(std::hash<std::string>{}(desc.fragmentPath) << 1);
		}
	};
}
