#pragma once
#include <functional>
#include <typeindex>
#include <cstdint>
#include <ostream>

namespace engine {
	namespace core {

		struct ResourceIdBase {
			uint32_t value;

			ResourceIdBase() = default;
			explicit ResourceIdBase(uint32_t value) : value(value) {}

			bool operator==(const ResourceIdBase& other) const { return value == other.value; }
			bool operator!=(const ResourceIdBase& other) const { return value != other.value; }
			operator bool() const { return value != 0; }
		};

		template<typename T>
		struct ResourceId : public ResourceIdBase {
			using ResourceIdBase::ResourceIdBase;

			ResourceId() = default;
			explicit ResourceId(uint32_t value): ResourceIdBase(value){}
			explicit ResourceId(const ResourceIdBase& base): ResourceIdBase(base.value){}

		};
	}
}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const engine::core::ResourceId<T>& resId) {
	using engine::core::TypeTag;
	std::type_index resourceType = engine::core::TypeTag<T>::index();

	std::string typeName = resourceType.name();

	// Clean MSVC type names
	size_t lastColon = typeName.rfind("::");
	if (lastColon != std::string::npos) {
		typeName = typeName.substr(lastColon + 2);
	}

	if (typeName.rfind("class ", 0) == 0) // ✅ equivalent to starts_with
		typeName = typeName.substr(6);

	if (typeName.rfind("struct ", 0) == 0)
		typeName = typeName.substr(7);
	if (!typeName.empty() && typeName.back() == '*') {
		typeName.pop_back();
		typeName.pop_back();
	}

	os << "[" << typeName << "ID: " << resId.value << "]";
	return os;
}

namespace std {
	template<>
	struct hash<engine::core::ResourceIdBase> {
		size_t operator()(const engine::core::ResourceIdBase& id) const noexcept {
			return std::hash<uint32_t>{}(id.value);
		}
	};
}

namespace std {
	template<typename T>
	struct hash<engine::core::ResourceId<T>> {
		size_t operator()(const engine::core::ResourceId<T>& id) const noexcept {
			size_t h1 = std::hash<uint32_t>{}(id.value);
			size_t h2 = std::hash<std::type_index>{}(typeid(T));
			return h1 ^ (h2 << 1); // combine hashes
		}
	};
}