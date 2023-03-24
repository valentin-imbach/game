
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

[[nodiscard]] inline float perlin(uint seed, vec position) {
	pair steps[4] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
	position.x += rand_float(seed + 1);
	position.y += rand_float(seed + 2);
	pair cell = floor(position);
	vec offset = position - cell;
	float dots[4] = {
		polar(rand_float(hash(seed, cell + steps[0]))) * (offset - steps[0]),
		polar(rand_float(hash(seed, cell + steps[1]))) * (offset - steps[1]),
		polar(rand_float(hash(seed, cell + steps[2]))) * (offset - steps[2]),
		polar(rand_float(hash(seed, cell + steps[3]))) * (offset - steps[3])};
	float left = dots[0] + smoothstep(offset.y) * (dots[2] - dots[0]);
	float right = dots[1] + smoothstep(offset.y) * (dots[3] - dots[1]);
	return left + smoothstep(offset.x) * (right - left);
}