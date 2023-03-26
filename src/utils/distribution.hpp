
#pragma once
#include "random.hpp"
#include "vec.hpp"

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

inline float smoothstep(float x) {
	return 6 * pow(x, 5) - 15 * pow(x, 4) + 10 * pow(x, 3);
}

[[nodiscard]] inline float perlin_octave(uint seed, vec position) {
	pair steps[4] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
	pair cell = floor(position);
	vec offset = position - cell;
	float dots[4] = {
		polar(2*M_PI*rand_float(hash(seed, cell + steps[0]))) * (offset - steps[0]),
		polar(2*M_PI*rand_float(hash(seed, cell + steps[1]))) * (offset - steps[1]),
		polar(2*M_PI*rand_float(hash(seed, cell + steps[2]))) * (offset - steps[2]),
		polar(2*M_PI*rand_float(hash(seed, cell + steps[3]))) * (offset - steps[3])};
	float left = dots[0] + smoothstep(offset.y) * (dots[2] - dots[0]);
	float right = dots[1] + smoothstep(offset.y) * (dots[3] - dots[1]);
	return left + smoothstep(offset.x) * (right - left);
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
