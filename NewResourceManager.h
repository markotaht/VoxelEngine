#pragma once
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <typeindex>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <utility>

#include "IAnyLoader.h"
#include "ResourceLoaderWrapper.h"
#include "NewResource.h"
#include "IResourceLoader.h"
#include "FileDescriptor.h"
#include "ResourceId.h"
#include "Types.h"
#include "TypeTag.h"

namespace engine {
    namespace resource {

        class ResourceManager {
        public:

            template<typename Loader>
            void registerLoader(std::unique_ptr<Loader> loader) {
                using Desc = typename Loader::DescriptorType;
                using Res = typename Loader::ResourceType;

                registerLoader<Desc, Res>(std::move(loader));
            }

            template<typename Desc, typename Res>
            void registerLoader(std::unique_ptr<loader::IResourceLoader<Desc, Res>> loader) {
                using engine::core::TypeTag;
                std::type_index resourceType = core::TypeTag<Res>::index();
                auto& vec = loaders[resourceType];
                vec.emplace_back(std::make_unique<loader::ResourceLoaderWrapper<Desc, Res>>(std::move(loader)));
            }

            template <typename Desc, typename Res>
            core::ResourceId<Res> load(const Desc& descriptor, std::string_view name = "") {
                using engine::core::TypeTag;

                std::type_index resourceType = core::TypeTag<Res>::index();
                Hash cacheKey = MakeHashKey(descriptor);

                auto it = hashToID.find(cacheKey);
                if (it != hashToID.end() && it->second.second == resourceType) {
                    return core::ResourceId<Res>(it->second.first);
                }
                
                std::type_index descriptorType = core::TypeTag<Desc>::index();

                auto loaderIt = loaders.find(resourceType);
                if (loaderIt == loaders.end()) {
                    std::cerr << "No loaders registered for resource type: " << resourceType.name() << "\n";
                    return core::ResourceId<Res>{0};
                }

                std::unique_ptr<Resource> resource = nullptr;
                for (const auto& loader : loaderIt->second) {
                    if (loader->canLoad(descriptorType, resourceType) && loader->canLoad(&descriptor)) {
                        resource = std::move(loader->load(&descriptor, resourceType));
                        break;
                    }
                }

                if (!resource) {
                    std::cerr << "No loader found for: " << descriptor << "\n";
                    return core::ResourceId<Res>{0};
                }

                engine::core::ResourceId<Res> id{nextID++};
                hashToID.emplace(cacheKey, std::make_pair( id , resourceType));
                if (!name.empty()) {
                    nameToID[resourceType][std::string(name)] = id;
                }
                idToType.emplace(id, resourceType);


                typedResources[resourceType][id] = std::move(resource);

                return id;
            }

            template <typename Res>
            core::ResourceId<Res> load(const std::string& path, std::string_view name = "") {
                return load<descriptor::FileDescriptor, Res>({ path }, name);
            }

            template <typename Res>
            core::ResourceId<Res> load(const descriptor::FileDescriptor& descriptor, std::string_view name = "") {
                return load<descriptor::FileDescriptor, Res>(descriptor, name);
            }

            template <typename Res>
            core::ResourceId<Res> add(std::unique_ptr<Res> resource, std::string_view name = "") {
                using engine::core::TypeTag;
                std::type_index resourceType = core::TypeTag<Res>::index();
                engine::core::ResourceId<Res> id{nextID++};

                if (!name.empty()) {
                    nameToID[resourceType][std::string(name)] = id;
                }
                idToType.emplace(id, resourceType);


                typedResources[resourceType][id] = std::move(resource);
                return id;
            }

            template <typename T>
            T* get(engine::core::ResourceId<T> id) {
                auto typeIt = idToType.find(id);
                if (typeIt == idToType.end()) return nullptr;

                std::type_index actualType = typeIt->second;

                auto tableIt = typedResources.find(actualType);
                if (tableIt == typedResources.end()) return nullptr;

                auto& table = tableIt->second;
                auto resIt = table.find(id);
                if (resIt == table.end()) return nullptr;

                return static_cast<T*>(resIt->second.get());
            }

            template <typename T>
            T* get(std::string_view name) {
                engine::core::ResourceId<T> id = getId<T>(name);
                if (!id) return nullptr; // only works if you add `operator bool()` or check `id != 0`
                return get<T>(id);
            }

            template<typename T>
            engine::core::ResourceId<T> getId(std::string_view name) {
                using engine::core::TypeTag;
                std::type_index resourceType = core::TypeTag<T>::index();
                auto nameIt = nameToID.find(resourceType);
                if (nameIt == nameToID.end()) return engine::core::ResourceId<T>{0};

                auto& nameTable = nameIt->second;
                auto resIt = nameTable.find(name);
                if (resIt == nameTable.end()) return engine::core::ResourceId<T>{0};
                return engine::core::ResourceId<T>{resIt->second};
            }

            template <typename T>
            void unload(engine::core::ResourceId<T> id) {
                auto it = idToType.find(id);
                if (it != idToType.end()) {
                    auto& table = typedResources[it->second];
                    auto resIt = table.find(id);
                    if (resIt != table.end()) {
                        resIt->second->unload();
                        table.erase(resIt);
                    }
                    idToType.erase(it);
                }
            }

            void unloadAll() {
                for (auto& [type, table] : typedResources) {
                    for (auto& [id, res] : table) {
                        res->unload();
                    }
                    table.clear();
                }
                typedResources.clear();
                hashToID.clear();
                idToType.clear();
            }

        private:
            std::unordered_map<std::type_index, std::vector<std::unique_ptr<loader::IAnyloader>>> loaders;

            uint32_t nextID = 1;
            std::unordered_map<Hash, std::pair<engine::core::ResourceIdBase, std::type_index>> hashToID;
            std::unordered_map<engine::core::ResourceIdBase, std::type_index> idToType;

            std::unordered_map<std::type_index, std::unordered_map<std::string, engine::core::ResourceIdBase>> nameToID;
            std::unordered_map<std::type_index, std::unordered_map<engine::core::ResourceIdBase, std::unique_ptr<Resource>>> typedResources;
        };
    }
}