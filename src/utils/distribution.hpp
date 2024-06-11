
#pragma once
#include "random.hpp"
#include "vec.hpp"
#include "lerp.hpp"

template <typename T>
[[nodiscard]] inline uint hash(T x) {
	return noise::UInt(uint(x));
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
	pair cell = vec::floor(position);
	vec offset = position - cell;
	float dots[4];
	for (int i = 0; i < 4; i++) {
		uint s = hash(seed, cell + steps[i]);
		dots[i] = vec::dot(vec::polar(noise::Float(s, -M_PI, M_PI)), offset - steps[i]);
	}
	float left = lerp::smooth(offset.y, dots[0], dots[2]);
	float right = lerp::smooth(offset.y, dots[1], dots[3]);
	return lerp::smooth(offset.x, left, right);
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
			offset.x = noise::Float(s++);
			offset.y = noise::Float(s++);
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

struct Allocation {
	virtual bool get(pair position) = 0;
	virtual ~Allocation() = default;
};

struct SpacedAllocation : public Allocation {
	uint seed;
	int space;
	int radius;
	pair shift;

	SpacedAllocation(uint seed, int space, int radius) : seed(seed), space(space), radius(radius) {
		shift.x = noise::Int(seed + 1, space);
		shift.y = noise::Int(seed + 2, space);
	}

	bool get(pair position) override {
		pair cell = space * pair((position + shift) / space);
		uint s = hash(seed, cell);
		float angle = noise::Float(s++, 2*M_PI);
		float dist = noise::Float(s++, radius);
		pair offset = vec::round(vec::polar(angle, dist));
		return position == cell + offset;
	}
};