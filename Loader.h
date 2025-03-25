#pragma once

#include <memory>
template <typename T>
class Loader {
public:
	virtual std::unique_ptr<T> loadResource(const char* path) = 0;
};