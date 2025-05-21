#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include <string_view>
#include <cstdint>

#include "Resource.h"
#include "IStorage.h"
#include "ResourceId.h"
#include "ResourceMetadata.h"
#include "ResourceReflection.h"
#include "FrameClock.h"
#include "LoaderKey.h"
#include "IResourceLoader.h"
#include "IGpuUploader.h"
#include "IGpuUploaderBase.h"
#include "OutStream.h"

namespace engine::resource {
	template<typename T>
	concept IsDerivedFromResource = std::derived_from<T, Resource>;

	template<IsDerivedFromResource Res>
	class ResourceStorage : public IStorage {
		struct ResourceEntry {
			std::unique_ptr<Res> instance;
			ResourceMetadata<Res> metadata;

			ResourceEntry(std::unique_ptr<Res> res, ResourceMetadata<Res> meta)
				: instance(std::move(res)), metadata(std::move(meta)) {
			}

			ResourceEntry() = default;
			ResourceEntry(ResourceEntry&&) = default;
			ResourceEntry& operator=(ResourceEntry&&) = default;

			ResourceEntry(const ResourceEntry&) = delete;
			ResourceEntry& operator=(const ResourceEntry&) = delete;
		};
	public:

		using ResourceMap = std::unordered_map<core::ResourceId<Res>, ResourceEntry>;
		ResourceStorage(ResourceManager& resMan):resMan(resMan){}

		template<typename Desc>
		void registerLoader(std::unique_ptr<loader::IResourceLoader<Desc, Res>> loader) {
			loader::LoaderKey loaderKey{ core::typeId<Desc>(), core::typeId<Res>() };
			loadersMap[loaderKey] = std::move(loader);
		}

		template<typename Desc>
		loader::IResourceLoader<std::remove_cvref_t<Desc>, Res>* getLoader() {
			loader::LoaderKey key{ core::typeId<Desc>(), core::typeId<Res>() };
			auto it = loadersMap.find(key);
			if (it != loadersMap.end()) {
				using CleanDesc = std::remove_cvref_t<Desc>;
				auto* wrapper = dynamic_cast<loader::IResourceLoader<CleanDesc, Res>*>(it->second.get());
				return wrapper ? wrapper : nullptr;
			}
			return nullptr;
		}

		loader::IGpuUploader<Res>* getGpuUploader(core::TypeId descriptorType) {
			loader::LoaderKey key{ descriptorType, core::typeId<Res>() };
			auto it = loadersMap.find(key);
			if (it != loadersMap.end()) {
				auto* wrapper = dynamic_cast<loader::IGpuUploader<Res>*>(it->second.get());
				return wrapper ? wrapper : nullptr;
			}
			return nullptr;
		}

		Res* get(core::ResourceId<Res> id, uint64_t frameIndex) {
			auto it = resources.find(id);
			if (it == resources.end()) return nullptr;
			it->second.metadata.lastUsedFrame = frameIndex;
			return it->second.instance.get();
		}

		Res* get(std::string_view name, uint64_t frameIndex) {
			auto it = nameToId.find(name);
			if (it == nameToId.end()) return nullptr;
			return get(it->second, frameIndex);
		}

		template<typename Desc>
		Res* load(const core::ResourceId<Res> id, Desc& descriptor, std::string_view name = "") {
			const auto resourceType = core::typeId<Res>();
			auto* loader = getLoader<Desc>();
			if (!loader) {
				std::cerr << "No loaders registered for resource type: " << core::getTypeName(resourceType) << "\n";
				return nullptr;
			}

			std::unique_ptr<Res> resource = loader->load(descriptor);
			if (!resource) {
				std::cerr << "Failed to load resource of type " << core::getTypeName(resourceType)
					<< " using descriptor: " << descriptor << "\n";
				return nullptr;
			}

			ResourceMetadata<Res> meta{core::typeId<Desc>()};
			if constexpr (util::HasGetResourceIds<Res>) {
				auto deps = resource->getResourceIds();
				util::forEachInTuple(deps, [&meta, this](auto& id) {
					resMan.addReference(id);
					meta.hardDependencies.push_back(id);
					});
			}

			auto* resPtr = resource.get();

			ResourceEntry entry{ std::move(resource) ,std::move(meta) };
			resources.try_emplace(id, std::move(entry));

			if (!name.empty()) {
				nameToId[std::string(name)] = id;
				idToName[id] = std::string(name);
			}
			return resPtr;
		}

		core::ResourceId<Res> add(engine::core::ResourceId<Res> id, std::unique_ptr<Res> resource, std::string_view name = "") {
			ResourceMetadata<Res> meta{ core::InvalidTypeId };
			meta.dirty = false;
			if constexpr (util::HasGetResourceIds<Res>) {
				auto deps = resource->getResourceIds();
				util::forEachInTuple(deps, [&meta, this](auto& id) {
					resMan.addReference(id);
					meta.hardDependencies.push_back(id);
					});
			}

			ResourceEntry entry{ std::move(resource) ,std::move(meta) };
			resources.try_emplace(id, std::move(entry));

			if (!name.empty()) {
				nameToId[std::string(name)] = id;
				idToName[id] = std::string(name);
			}
			return id;
		}

		bool unloadResource(core::ResourceId<Res> id) {
			auto it = resources.find(id);
			if (it == resources.end()) return false;

			if (it->second.metadata.refCount == 0) return false;
			unloadResource(it->second);
			unregisterResource(it->first);
			resources.erase(id);
			return true;
		}

		std::vector<core::ResourceIdBase> unloadUnused() override{
			std::vector<core::ResourceIdBase> removedIds;
			for (auto id : unloadQueue) {
				unloadResource(id);
				unregisterResource(id);
				removedIds.push_back(id);
			}
			return removedIds;
		}

		void unloadAll() override {
			for (auto& [id, entry] : resources) {
				unloadResource(entry);
			}
			resources.clear();
			nameToId.clear();
			idToName.clear();
		};

		void reloadAll() override {};

		void uploadIfDirty(core::ResourceId<Res> id) {
			typename ResourceMap::iterator it = resources.find(id);
			if (it == resources.end()) return;
			uploadIfDirty(it->second);
		}

		void uploadIfDirty(ResourceEntry& entry) {
			if (entry.metadata.dirty) {
				auto* loader = getGpuUploader(entry.metadata.descriptorType);
				if (!loader) {
					std::cerr << "No loader found for upload of resource\n";
					return;
				}

				loader->uploadGPU(*entry.instance.get());
				entry.metadata.dirty = false;
				entry.metadata.estimatesMemoryUsage = entry.instance->estimateMemoryUsage();
				entry.metadata.estimatedGpuMemoryUsage = entry.instance->estimateGpuMemoryUsage();

			/*	std::cout << entry.instance->toString() << "\n";
				std::cout << entry.metadata.formatMetadata() << "\n";*/
			}
		}

		void uploadAllDirty() override{
			for (auto& [_, entry] : resources) {
				uploadIfDirty(entry);
			}
		}

		size_t estimateMemoryUsage() override{
			size_t total = 0;
			for (const auto& [_, entry] : resources) {
				total += entry.metadata.estimatesMemoryUsage;
			}
			return total;
		}

		size_t estimateGPUMemoryUsage() override{
			size_t total = 0;
			for (const auto& [_, entry] : resources) {
				total += entry.metadata.estimatedGpuMemoryUsage;
			}
			return total;
		}

		ResourceMetadata<Res>* getMetadata(core::ResourceIdBase id) {
			auto resId = core::convertId<Res>(id);
			auto it = resources.find(resId);
			if (it == resources.end()) return nullptr;
			return &it->second.metadata;
		}

		void markDirty(core::ResourceIdBase id) override {
			if (ResourceMetadata<Res>* meta = getMetadata(id)) {
				meta->dirty = true;
			}
		}

		bool isDirty(core::ResourceIdBase id) {
			if (ResourceMetadata<Res>* meta = getMetadata(id)) {
				return meta->dirty;
			}
			return false;
		}

		void clearDirty(core::ResourceIdBase id) {
			if (ResourceMetadata<Res>* meta = getMetadata(id)) {
				meta->dirty = false;
			}
		}

		void addReference(core::ResourceIdBase id) {
			if (ResourceMetadata<Res>* meta = getMetadata(id)) {
				meta->refCount++;
			}
		}

		void removeReference(core::ResourceIdBase id) {
			if (ResourceMetadata<Res>* meta = getMetadata(id)) {
				meta->refCount--;
			}
		}

		void setLastSeenFrame(core::ResourceIdBase id, uint64_t frameIndex) {
			if (ResourceMetadata<Res>* meta = getMetadata(id)) {
				meta->lastUsedFrame = frameIndex;
			}
		}

		void tick(uint64_t frameIndex) {
			unloadQueue.clear();
			for(auto& [id, entry]: resources){
				ResourceMetadata<Res>& meta = entry.metadata;
				if (meta.refCount == 0 && frameIndex >= meta.lastUsedFrame &&
					(frameIndex - meta.lastUsedFrame) > unusedTreshold) {
					unloadQueue.push_back(id);
				}
			}
		}

		void debugPrint(core::OutStream& out) const {
			for (auto& [id, entry] : resources) {
				out << id << entry.metadata.formatMetadata() << "\n";
			}
		}

	private:
		ResourceMap resources;
		ResourceManager& resMan;
		uint64_t unusedTreshold = 4000;

		std::unordered_map<loader::LoaderKey, std::unique_ptr<loader::IGpuUploaderBase>, loader::LoaderKeyHasher> loadersMap;
		std::unordered_map<std::string, core::ResourceId<Res>> nameToId;
		std::unordered_map<core::ResourceId<Res>, std::string> idToName;
		std::vector<core::ResourceId<Res>> unloadQueue;


		void unloadResource(ResourceEntry& entry) {
			if constexpr (engine::util::HasGetResourceIds<Res>) {
				auto& manager = resMan;
				util::forEachResourceId(*entry.instance.get(), [&manager](auto& id) {
					manager.removeReference(id);
					//resMan.removeDependency(id);
					});
			}
			entry.instance->unload();
		}

		void unregisterResource(core::ResourceId<Res> id) {
			auto it = idToName.find(id);
			if (it != idToName.end()) {
				std::string name = it->second;
				nameToId.erase(name);
				idToName.erase(it);
			}
		}
	};
}