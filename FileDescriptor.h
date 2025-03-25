#pragma once
#include <string>
#include <ostream>

namespace engine {
	namespace descriptor {
		struct FileDescriptor {
			std::string filePath;

			bool operator==(const FileDescriptor& other) const {
				return filePath == other.filePath;
			}
		};
	}
}

inline std::ostream& operator<<(std::ostream& os, const engine::descriptor::FileDescriptor& desc) {
	os << desc.filePath;
	return os;
}

namespace std {
	template<>
	struct hash<engine::descriptor::FileDescriptor> {
		std::size_t operator()(const engine::descriptor::FileDescriptor& desc) const {
			return std::hash<std::string>{}(desc.filePath);
		}
	};
}