#pragma once
#include <limits>
#include <vector>
#include <cassert>
#include "typedefs.hpp"

namespace noise {
	[[nodiscard]] inline uint UInt(uint seed) {
		uint x = seed + 23782755;
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		return (x >> 16) ^ x;
	}

	[[nodiscard]] inline float Float(uint seed) {
		return float(UInt(seed)) / float(std::numeric_limits<uint>::max());
	}

	[[nodiscard]] inline float Float(uint seed, float min, float max) {
		assert(min <= max);
		return min + Float(seed) * (max - min);
	}

	[[nodiscard]] inline float Float(uint seed, float max) {
		return Float(seed, 0, max);
	}

	[[nodiscard]] inline int Int(uint seed, int min, int max) {
		assert(min < max);
		return min + UInt(seed) % (max - min);
	}

	[[nodiscard]] inline int Int(uint seed, int max) {
		return Int(seed, 0, max);
	}

	[[nodiscard]] inline bool Bool(uint seed) {
		return UInt(seed) % 2;
	}

	//* utility

	[[nodiscard]] inline bool bernoulli(uint seed, float p) { 
		return Float(seed) < p;
	}

	template <typename T>
	[[nodiscard]] inline T choice(uint seed, std::vector<T> &vec) {
		uint index = Int(seed, 0, vec.size());
		return vec[index];
	}

	template <typename T>
	[[nodiscard]] inline T choice(uint seed, std::vector<T> &&vec) {
		uint index = Int(seed, 0, vec.size());
		return vec[index];
	}
}