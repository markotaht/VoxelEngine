#pragma once
#include <functional>

#include "TypeId.h"

namespace engine::loader {
	struct LoaderKey {
		core::TypeId descriptorTypeId;
		core::TypeId resourceTypeId;

		bool operator==(const LoaderKey& other) const {
			return descriptorTypeId == other.descriptorTypeId && resourceTypeId == other.resourceTypeId;
		}
	};

	struct LoaderKeyHasher {
		std::size_t operator()(const LoaderKey& key) const {
			std::size_t h1 = std::hash<core::TypeId>()(key.resourceTypeId);
			std::size_t h2 = std::hash<core::TypeId>()(key.descriptorTypeId);
			return h1 ^ (h2 << 1);
		}
	};
}