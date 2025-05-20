#pragma once
#include <typeindex>
#include <type_traits>

namespace engine::util {
    template<typename T>
    inline std::type_index getTypeIndex() {
        using CleanT = std::remove_cvref_t<T>;
        return std::type_index(typeid(CleanT));
    }
}