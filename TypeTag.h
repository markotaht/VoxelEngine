#pragma once
#include <typeindex>

namespace engine::core {
    template<typename T>
    struct TypeTag {
        static std::type_index index() {
            return typeid(T*);
        }
    };
}