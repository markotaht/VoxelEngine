#pragma once
#include <unordered_map>
#include <string>
#include <cstddef>

namespace engine::core {
	using TypeId = size_t;
    constexpr TypeId InvalidTypeId = 0;

    template<typename T>
    inline TypeId cleanTypeId() {
        static char unique_for_type;
        return reinterpret_cast<std::size_t>(&unique_for_type);
    }

    template<typename T>
    inline TypeId typeId() {
        using CleanT = std::remove_cvref_t<T>;
        return cleanTypeId<CleanT>();
    }

    inline std::unordered_map<TypeId, std::string>& getTypeNameMap() {
        static std::unordered_map<TypeId, std::string> map;
        return map;
    }

    template <typename T>
    constexpr std::string_view TypeName() {
#if defined(__clang__)
        constexpr std::string_view p = __PRETTY_FUNCTION__;
        constexpr auto start = p.find("T = ") + 4;
        constexpr auto end = p.rfind("]");
        return p.substr(start, end - start);
#elif defined(__GNUC__)
        constexpr std::string_view p = __PRETTY_FUNCTION__;
        constexpr auto start = p.find("T = ") + 4;
        constexpr auto end = p.find_first_of(';', start);
        return p.substr(start, end - start);
#elif defined(_MSC_VER)
        constexpr std::string_view p = __FUNCSIG__;
        constexpr auto start = p.find("TypeName<") + 9;
        constexpr auto end = p.find_last_of('>');
        return p.substr(start, end - start);
#else
        return "UnknownType";
#endif
    }

    inline std::string getTypeName(TypeId id) {
        auto& map = getTypeNameMap();
        auto it = map.find(id);
        return it != map.end() ? it->second : "<unknown>";
    }
}