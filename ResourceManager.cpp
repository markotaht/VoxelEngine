#include "ResourceManager.h"

namespace engine::resource {
	void ResourceManager::uploadAllDirty() {
		for (auto& [_, storage] : storages) {
			storage->uploadAllDirty();
		}
	}

	void ResourceManager::tickResource(uint64_t frameIndex) {
		for (auto& [_, storage] :storages) {
			storage->tick(frameIndex);
		}
	}

	void ResourceManager::unloadAll() {
		for (auto& [_, storage] : storages) {
			storage->unloadAll();
		}

		hashToId.clear();
		idToHash.clear();
	}

	void ResourceManager::unloadUnused() {
		for (auto&& [_, storage] : storages) {
			std::vector<core::ResourceIdBase> ids = std::move(storage->unloadUnused());
			for (auto& id : ids) {
				unregisterResource(id);
			}
		}
	}

   void ResourceManager::unregisterResource(core::ResourceIdBase id) {
        auto it = idToHash.find(id);
        if (it != idToHash.end()) {
            Hash hash = it->second;
            hashToId.erase(hash);
            idToHash.erase(it);
        }
    }

    IStorage* ResourceManager::getStorageByResourceId(core::ResourceIdBase id) {
        auto storageIt = storages.find(id.typeId);
        if (storageIt == storages.end()) return nullptr;

        return storageIt->second;
    }

   void ResourceManager::markDirty(core::ResourceIdBase id) {
        if (auto* storage = getStorageByResourceId(id)) {
            storage->markDirty(id);
        }
    }

    bool ResourceManager::isDirty(core::ResourceIdBase id) {
        if (auto* storage = getStorageByResourceId(id)) {
            return storage->isDirty(id);
        }
        return false;
    }

   void ResourceManager::clearDirty(core::ResourceIdBase id) {
        if (auto* storage = getStorageByResourceId(id)) {
            storage->clearDirty(id);
        }
    }

   void ResourceManager::addReference(core::ResourceIdBase id) {
        if (auto* storage = getStorageByResourceId(id)) {
            storage->addReference(id);
        }
    }

   void ResourceManager::removeReference(core::ResourceIdBase id) {
        if (auto* storage = getStorageByResourceId(id)) {
            storage->removeReference(id);
        }
    }

   void ResourceManager::setLastSeenFrame(core::ResourceIdBase id) {
        if (frameClock) {
            auto* storage = getStorageByResourceId(id);
            if (storage) {
                storage->setLastSeenFrame(id, frameClock->getFrame());
            }
        }
    }

   void ResourceManager::trackDependency(entity::Entity entity, core::ResourceIdBase id) {

    }

   void ResourceManager::removeDependency(core::ResourceIdBase id) {

    }

    size_t ResourceManager::estimateMemoryUsage() {
        size_t total = 0;
        for (auto& [_, storage] : storages) {
            total += storage->estimateMemoryUsage();
        }
        return total;
    }

    size_t ResourceManager::estimateGPUMemoryUsage() {
        size_t total = 0;
        for (auto& [_, storage] : storages) {
            total += storage->estimateGPUMemoryUsage();
        }
        return total;
    }

    void ResourceManager::debugPrint(core::OutStream& out) const {
        for (auto& [_, storage] : storages) {
            storage->debugPrint(out);
        }
    }

    void ResourceManager::shutdown() {
        unloadAll();
    }
}