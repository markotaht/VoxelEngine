#pragma once
#include <string>

namespace engine {
	namespace resource {
		class Resource {
		public:
			virtual ~Resource() = default;
			virtual void unload() = 0;
		};
	}
}