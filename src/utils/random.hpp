#pragma once
#include <limits>
#include <vector>

typedef unsigned int uint;

//* uint

[[nodiscard]] inline uint rand_uint(uint seed) {
	uint x = seed + 23782755;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	return (x >> 16) ^ x;
}

//* float

[[nodiscard]] inline float rand_float(uint seed) {
	return float(rand_uint(seed)) / float(std::numeric_limits<uint>::max());
}

[[nodiscard]] inline float rand_float(uint seed, float min, float max) {
	assert(min <= max);
	return min + rand_float(seed) * (max - min);
}

// * int

[[nodiscard]] inline int rand_int(uint seed, int min, int max) {
	assert(min < max);
	return min + rand_uint(seed) % (max - min);
}

//* bool

[[nodiscard]] inline bool rand_bool(uint seed) { return rand_uint(seed) % 2; }
[[nodiscard]] inline bool bernoulli(uint seed, float p) { return rand_float(seed) < p; }

//* utility

template <typename T>
[[nodiscard]] inline T rand_choice(uint seed, std::vector<T> &vec) {
	uint index = rand_int(seed, 0, vec.size());
	return vec[index];
}

template <typename T>
[[nodiscard]] inline T rand_choice(uint seed, std::vector<T> &&vec) {
	uint index = rand_int(seed, 0, vec.size());
	return vec[index];
}
