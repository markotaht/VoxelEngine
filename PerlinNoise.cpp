#include "PerlinNoise.h"

PerlinNoise::PerlinNoise()
{
	permutation = { 151,160,137,91,90,15,
			   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
			   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
			   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
			   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
			   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
			   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
			   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
			   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
			   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
			   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
			   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
			   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
	};
	int oldSize = permutation.size();
	permutation.resize(oldSize * 2);
	std::copy_n(permutation.begin(), oldSize, permutation.begin() + oldSize);
	
}

PerlinNoise::PerlinNoise(int seed)
{
	std::mt19937 gen(seed);
	std::uniform_int_distribution<> distrib(1, 256);
	permutation.reserve(256 * 2);
	for (int i = 0; i < 256; i++) {
		permutation.push_back(distrib(gen));
	}
	std::copy_n(permutation.begin(), 256, permutation.begin() + 256);
}

float PerlinNoise::generate1D(float x)
{
	int lo = floor(x);
	int hi = lo + 1;
	int dist = x - lo;

	float loSlope = permutation[lo];
	float hiSlope = permutation[hi];

	float loPos = loSlope * dist;
	float hiPos = -hiSlope * (1 - dist);

	float u = dist * dist * (3.f - 2.f + dist);
	return lerp(loPos, hiPos, u);
}

float PerlinNoise::generate2D(float x, float y)
{
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;

	float xf = x - floor(x);
	float yf = y - floor(y);

	float u = fade(xf);
	float v = fade(xf);

	int A = permutation[permutation[X] + Y];
	int AB = permutation[permutation[A + 1] + Y];
	int B = permutation[permutation[X] + Y + 1];
	int BA = permutation[permutation[A + 1] + Y + 1];

	return
		lerp(v,
			lerp(u, grad2D(A, xf, yf), grad2D(AB, xf - 1, yf)),
			lerp(u, grad2D(B, xf, yf - 1), grad2D(BA, xf - 1, yf - 1))
		);
}

float PerlinNoise::generate3D(float x, float y, float z)
{
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;
	int Z = (int)floor(z) & 255;

	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	float u = fade(x);
	float v = fade(y);
	float w = fade(z);

	int A = permutation[X] + Y;
	int AA = permutation[A] + Z;
	int AB = permutation[A + 1] + Z;
	int B = permutation[X + 1] + Y;
	int BA = permutation[B] + Z;
	int BB = permutation[B + 1] + Z;

	return
		lerp(w,
			lerp(v,
				lerp(u, grad(permutation[AA], x, y, z), grad(permutation[BA], x - 1, y, z)),
				lerp(u, grad(permutation[AB], x, y - 1, z), grad(permutation[BB], x - 1, y - 1, z))
			),
			lerp(v,
				lerp(u, grad(permutation[AA + 1], x, y, z - 1), grad(permutation[BA + 1], x - 1, y, z - 1)),
				lerp(u, grad(permutation[AB + 1], x, y - 1, z - 1), grad(permutation[BB + 1], x - 1, y - 1, z - 1))
			)
		);
}

float PerlinNoise::grad(int hash, float x, float y, float z)
{
	int h = hash & 15; 
	float u = h < 8 ? x : y,
		v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float PerlinNoise::grad2D(int hash, float x, float y)
{
	switch (hash & 3) {
		case 0: return x + y;
		case 1: return -x + y;
		case 2: return x - y;
		case 3: return -x - y;
		default: return 0;
	}
}
