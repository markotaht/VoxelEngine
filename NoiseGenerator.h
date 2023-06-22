#pragma once
class NoiseGenerator {
public:
	virtual float generate1D(float x) = 0;
	virtual float generate2D(float x, float y) = 0;
	virtual float generate3D(float x, float y, float z) = 0;
};