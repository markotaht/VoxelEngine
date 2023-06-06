#pragma once
template <typename T>
class Loader {
public:
	virtual T* loadResource(const char* path) = 0;
};