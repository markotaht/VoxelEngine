#pragma once
#include "NoiseGenerator.h"

#include <vector>
#include <cmath>
#include <random>
class PerlinNoise :
	public NoiseGenerator
{
public:
	PerlinNoise();
	PerlinNoise(int seed);

	float generate1D(float x);
	float generate2D(float x, float y);
	float generate3D(float x, float y, float z);
private:
	std::vector<int> permutation;

	inline float lerp(float low, float high, float v) { return (1 - v) * low + v * high; };
	inline float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }
	float grad(int hash, float x, float y, float z);
	float grad2D(int hash, float x, float y);
};

