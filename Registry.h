#pragma once
#include "Entity.h"
#include "ComponentStorage.h"
#include "IComponentStorage.h"

namespace engine::entity {
	class Registry {
    public:
        Entity nextEntity = 1;

        template<typename T>
        component::ComponentStorage<T>& getStorage() {
            static component::ComponentStorage<T> storage;
            return storage;
        }

        Entity create() {
            return nextEntity++;
        }

        template<typename T>
        void add(Entity e, const T& component) {
            getStorage<T>().add(e, component);
        }

        template<typename T>
        T* get(Entity e) {
            return getStorage<T>().get(e);
        }

        template<typename T>
        bool has(Entity e) const {
            return getStorage<T>().has(e);
        }

        template<typename T>
        void remove(Entity e) {
            getStorage<T>().remove(e);
        }

        // Iterate helper (basic)
      /*  template<typename T, typename Func>
        void each(Func func) {
            auto& storage = getStorage<T>();
            for (size_t i = 0; i < storage.size(); ++i)
                func(storage.getEntities()[i], storage.getComponents()[i]);
        }*/

        template<typename First, typename... Rest>
        component::IComponentStorage& getSmallestStorage(First& first, Rest&... rest) {
            component::IComponentStorage* smallest = &first;
            size_t minSize = smallest->size();

            (([&] {
                if (rest.size() < minSize) {
                    smallest = &rest;
                    minSize = rest.size();
                }
                }()), ...);

            return *smallest;
        }

        template<typename First, typename... Rest, typename Func>
        void each(Func func) {
            component::IComponentStorage& baseStorageIface = getSmallestStorage(getStorage<First>(), getStorage<Rest>()...);

            auto* basePtr = static_cast<component::ComponentStorage<First>*>(&baseStorageIface); // Step 1
            auto& baseStorage = *basePtr;

            for (size_t i = 0; i < baseStorage.size(); ++i) {
                Entity e = baseStorage.getEntities()[i];

                std::tuple<First*, Rest*...> refs = { getStorage<First>().get(e), getStorage<Rest>().get(e)... };

                if ((std::get<First*>(refs) != nullptr) && (... && (std::get<Rest*>(refs) != nullptr))) {
                    func(e, *std::get<First*>(refs), *std::get<Rest*>(refs)...);
                }
            }
        }
	};
}