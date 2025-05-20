#pragma once
#include <string>

namespace engine {
	namespace resource {
		class Resource {
		public:
			virtual size_t estimateMemoryUsage() const { return 0; }
			virtual size_t estimateGpuMemoryUsage() const { return 0; }
			virtual ~Resource() = default;
			virtual void unload() = 0;
			virtual std::string toString() = 0;
		};
	}
}