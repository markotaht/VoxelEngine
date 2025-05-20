#pragma once
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <typeindex>
#include <unordered_map>
#include <memory>
#include <iostream>

#include "ResourceId.h"
#include "Resource.h"

#include "IResourceLoaderBase.h"
#include "IResourceLoader.h"

#include "FileDescriptor.h"

#include "Types.h"
#include "TypeId.h"

#include "Entity.h"
#include "FrameClock.h"

#include "IStorage.h"
#include "ResourceStorage.h"
#include "LoaderKey.h"


namespace engine {
    namespace resource {
        
        class ResourceManager {
        public:
            static ResourceManager& instance() {
                static ResourceManager resMan;
                return resMan;
            }
            
            ~ResourceManager() {
            }

            inline void setFrameClock(core::FrameClock* frameClock) {
                this->frameClock = frameClock;
            }

            template<typename Desc, typename Res>
            void registerLoader(std::unique_ptr<loader::IResourceLoader<Desc, Res>> loader) {
                auto& storage = getStorage<Res>();
                storage.registerLoader<Desc>(std::move(loader));
            }

            template <typename Res>
            resource::ResourceStorage<Res>& getStorage() {
                static resource::ResourceStorage<Res> storage{ *frameClock, instance() };
                static bool once = [] {
                    auto& self = instance();
                    const core::TypeId resourceType = core::typeId<Res>();
                    self.storages[resourceType] = &storage;
                    return true;
                    }();
                return storage;
            }

            template <typename Desc, typename Res>
            core::ResourceId<Res> load(const Desc& descriptor, std::string_view name = "") {
                const auto resourceType = core::typeId<Res>();
                const auto descriptorType = core::typeId<Desc>();

                Hash cacheKey = MakeHashKey(descriptor);

                auto it = hashToId.find(cacheKey);
                if (it != hashToId.end() && it->second.typeId == resourceType) {
                    return core::ResourceId<Res>(it->second);
                };

                engine::core::ResourceId<Res> id{ nextID++ };
                resource::ResourceStorage<Res>& storage = getStorage<Res>();
                storage.load(id, descriptor, name);

                hashToId.emplace(cacheKey, id);
                idToHash.emplace(id, cacheKey);
                
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
                const core::TypeId resourceType = core::typeId<Res>();
                engine::core::ResourceId<Res> id{nextID++};

                resource::ResourceStorage<Res>& storage = getStorage<Res>();
                storage.add(id, std::move(resource), name);

                return id;
            }

            template <typename T>
            T* get(engine::core::ResourceId<T> id) {
                ResourceStorage<T>& storage = getStorage<T>();
                return storage.get(id);
            }

            template <typename T>
            T* get(std::string_view name) {
                ResourceStorage<T>& storage = getStorage<T>();
                return storage.get(name);
            }

            template <typename T>
            void uploadIfDirty(core::ResourceId<T> id) {
                resource::ResourceStorage<T>& storage = getStorage<T>();
                storage.uploadIfDirty(id);
            }

            void uploadAllDirty() {
                for (auto& [_, storage] : storages) {
                    storage->uploadAllDirty();
                }
            }

            void unloadAll() {
                for (auto& [_, storage] : storages) {
                    storage->unloadAll();
                }

                hashToId.clear();
                idToHash.clear();
            }

            void unloadUnused() {
                for (auto&& [_, storage] : storages) {
                    std::vector<core::ResourceIdBase> ids = std::move(storage->unloadUnused());
                    for (auto& id : ids) {
                        unregisterResource(id);
                    }
                }
            }

            template <typename T>
            void unload(engine::core::ResourceId<T> id) {
                resource::ResourceStorage<T>& storage = getStorage<T>();
                storage.unloadResource(id);
                unregisterResource(id);
            }

            void unregisterResource(core::ResourceIdBase id) {
                auto it = idToHash.find(id);
                if (it != idToHash.end()) {
                    Hash hash = it->second;
                    hashToId.erase(hash);
                    idToHash.erase(it);
                }
            }

            IStorage* getStorageByResourceId(core::ResourceIdBase id) {
                auto storageIt = storages.find(id.typeId);
                if (storageIt == storages.end()) return nullptr;

                return storageIt->second;
            }

            void markDirty(core::ResourceIdBase id) {
                if (auto* storage = getStorageByResourceId(id)) {
                    storage->markDirty(id);
                }
            }

            bool isDirty(core::ResourceIdBase id) {
                if (auto* storage = getStorageByResourceId(id)) {
                    return storage->isDirty(id);
                }
                return false;
            }

            void clearDirty(core::ResourceIdBase id) {
                if (auto* storage = getStorageByResourceId(id)) {
                    storage->clearDirty(id);
                }
            }

            void addReference(core::ResourceIdBase id) {
                if (auto* storage = getStorageByResourceId(id)) {
                    storage->addReference(id);
                }
            }

            void removeReference(core::ResourceIdBase id) {
                if (auto* storage = getStorageByResourceId(id)) {
                    storage->removeReference(id);
                }
            }

            void setLastSeenFrame(core::ResourceIdBase id) {
                if (frameClock) {
                    auto* storage = getStorageByResourceId(id);
                    if (storage) {
                        storage->setLastSeenFrame(id);
                    }
                }
            }

            void trackDependency(entity::Entity entity, core::ResourceIdBase id) {

            }

            void removeDependency(core::ResourceIdBase id) {

            }

            size_t estimateMemoryUsage() {
                size_t total = 0;
                for (auto& [_, storage] : storages) {
                    total += storage->estimateMemoryUsage();
                }
                return total;
            }

            size_t estimateGPUMemoryUsage() {
                size_t total = 0;
                for (auto& [_, storage] : storages) {
                    total += storage->estimateGPUMemoryUsage();
                }
                return total;
            }

            void shutdown() {
                unloadAll();
            }

        private:
            core::FrameClock* frameClock;
            uint64_t unusedTreshold = 4000;

            uint32_t nextID = 1;
            std::unordered_map<Hash, engine::core::ResourceIdBase> hashToId;
            std::unordered_map<engine::core::ResourceIdBase, Hash> idToHash;

            std::unordered_map<core::TypeId, resource::IStorage*> storages;
            std::unordered_map<loader::LoaderKey, std::unique_ptr<loader::IResourceLoaderbase>, loader::LoaderKeyHasher> loadersMap;
        };
    }
}