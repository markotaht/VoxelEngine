#pragma once
#include <type_traits>
#include <concepts>
#include <tuple>
#include <utility>

namespace engine::util {
    #define REFLECT_RESOURCE_IDS(...) \
        auto getResourceIds() const { return std::tie(__VA_ARGS__); }

    template<typename T>
    concept HasGetResourceIds = requires(const T & obj) {
        { obj.getResourceIds() };
    };

    template<typename T>
    concept IsContainer = requires(const T & t) {
        std::begin(t);
        std::end(t);
    } && !std::is_same_v<std::decay_t<T>, std::string>;

    // 1. Base case for ResourceId — invoke f
    template<typename T, typename F>
        requires (!IsContainer<T> && !HasGetResourceIds<T>)
    void forEachResourceId(const engine::core::ResourceId<T>& id, F&& f) {
        f(id);
    }

    template<typename T, typename F>
        requires IsContainer<T> && (!HasGetResourceIds<T>)
    void forEachResourceId(const T& container, F&& f) {
        for (const auto& element : container) {
            if constexpr (requires { element.second; }) {
                // Assume it's a pair like std::map or std::unordered_map
                forEachResourceId(element.second, std::forward<F>(f));
            }
            else {
                forEachResourceId(element, std::forward<F>(f));
            }
        }
    }

    // 2. Reflectable types that expose getResourceIds()
    template<typename T, typename F>
        requires HasGetResourceIds<T>
    void forEachResourceId(const T& obj, F&& f) {
        std::apply([&](auto&&... ids) {
            ((forEachResourceId(ids, f)), ...); // recursively visit children
            }, obj.getResourceIds());
    }

    template<typename T>
    struct always_false : std::false_type {};

    template<typename T, typename F>
    void forEachResourceId(const T&, F&&) {
        static_assert(always_false<T>::value, "forEachResourceId not supported for this type");
    }


    template <typename T, typename Func>
    requires (!IsContainer<T>)
    void handle(const T& value, Func&& func) {
        func(value);
    }

    template <typename T, typename Func>
        requires IsContainer<T>
    void handle(const T& container, Func&& func) {
        for (const auto& value : container) {
            if constexpr (requires { value.second; }) {
                func(value.second);
            }
            else {
                func(value);
            }
        }
    }


    template<typename Tuple, typename Func, std::size_t... Indices>
    void forEachInTupleImpl(Tuple&& tuple, Func&& func, std::index_sequence<Indices...>) {
        (handle(std::get<Indices>(tuple), std::forward<Func>(func)), ...); // fold expression
    }

    template<typename Tuple, typename Func>
    void forEachInTuple(Tuple&& tuple, Func&& func) {
        constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
        forEachInTupleImpl(std::forward<Tuple>(tuple), std::forward<Func>(func), std::make_index_sequence<N>{});
    }
}