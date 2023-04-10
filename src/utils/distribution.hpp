
#pragma once
#include "random.hpp"
#include "vec.hpp"
#include "lerp.hpp"

template <typename T>
[[nodiscard]] inline uint hash(T x) {
	return rand_uint(uint(x));
}

template <typename T, typename... TT>
[[nodiscard]] inline uint hash(T first, TT... args) {
	return hash(first) ^ (hash(args...) << 1);
}

template <>
[[nodiscard]] inline uint hash(pair x) {
	return hash(x.x, x.y);
}

[[nodiscard]] inline float perlin_octave(uint seed, vec position) {
	pair steps[4] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
	pair cell = floor(position);
	vec offset = position - cell;
	float dots[4];
	for (int i = 0; i < 4; i++) {
		uint s = hash(seed, cell + steps[i]);
		dots[i] = polar(rand_float(s, -M_PI, M_PI)) * (offset - steps[i]);
	}
	float left = Lerp::smooth(offset.y, dots[0], dots[2]);
	float right = Lerp::smooth(offset.y, dots[1], dots[3]);
	return Lerp::smooth(offset.x, left, right);
}

struct Distribution {
	virtual float get(vec position) = 0;
	virtual ~Distribution() = default;
};

struct PerlinNoise : public Distribution {
	uint seed;
	float per, amp, mean;
	int oct;
	PerlinNoise(uint seed = 0, float per = 1, float amp = 1, float mean = 0, int oct = 1) : seed(seed), per(per), amp(amp), mean(mean), oct(oct) {}

	float get(vec position) override {
		uint s = seed;
		float p = per;
		float a = amp;
		float res = mean;
		for (int i = 0; i < oct; i++) {
			vec offset;
			offset.x = rand_float(s++);
			offset.y = rand_float(s++);
			res += a * perlin_octave(s++, offset + position / p);
			p /= 2;
			a /= 2;
		}
		return res;
	}
};

struct BoundDistribution : public Distribution {
	std::unique_ptr<Distribution> distribution;
	float min, max;
	BoundDistribution(std::unique_ptr<Distribution> dist, float min, float max) : distribution(std::move(dist)), min(min), max(max) {}
	float get(vec position) override {
		return std::max(min, std::min(max, distribution->get(position)));
	}
};
