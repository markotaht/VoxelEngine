#pragma once
#include <vector>

namespace engine::entity {
	
	template<typename T>
	class ComponentArray : public IComponentArray {
		std::vector<T> data;
	public:
		void add(const T& t){
			data.push_back(t);
		}

		void addRaw(const void* raw) override {
			data.push_back(*reinterpret_cast<const T*>(raw));
		}

		void* getRawData() override {
			return data.data();
		}

		T* getTypedData(){
			return data.data();
		}
	};
}