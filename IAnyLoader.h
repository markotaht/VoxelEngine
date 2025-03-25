#pragma once
#include <memory>
#include <string>
#include <typeindex>

#include "NewResource.h"

namespace engine {
    namespace loader {
        class IAnyloader {
        public:
            virtual ~IAnyloader() = default;

            virtual bool canLoad(const void* descriptor) const = 0;
            virtual bool canLoad(std::type_index descriptorType, std::type_index resourceType) const = 0;

            // Return type-erased unique_ptr<Resource>
            virtual std::unique_ptr<resource::Resource> load(const void* descriptor, std::type_index type) const = 0;

        };
    }
}