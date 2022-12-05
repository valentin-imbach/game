#pragma once
#include "pair.hpp"
#include <cassert>
#include <cmath>

struct vec {
	float x, y;

	vec() : x(0), y(0) {}
	vec(pair p) : x(float(p.x)), y(float(p.y)) {}
	template <typename T1, typename T2>
	vec(T1 x, T2 y) : x(x), y(y) {}
};

//* addition

[[nodiscard]] inline vec operator+(vec v1, vec v2) {
	return {v1.x + v2.x, v1.y + v2.y};
}
inline vec operator+=(vec &v1, vec v2) { return v1 = v1 + v2; }

//* multiplication

[[nodiscard]] inline vec operator*(float t, vec v) {
	return {t * v.x, t * v.y};
}
[[nodiscard]] inline vec operator*(vec v, float t) { return t * v; }
inline vec operator*=(vec &v, float t) { return v = t * v; }

//* subtraction

[[nodiscard]] inline vec operator-(vec v) { return -1 * v; }
[[nodiscard]] inline vec operator-(vec v1, vec v2) { return v1 + (-v2); }
inline vec operator-=(vec v1, vec v2) { return v1 += (-v2); }

//* division

[[nodiscard]] inline vec operator/(vec v, float t) {
	assert(t != 0);
	return {v.x / t, v.y / t};
}

inline vec operator/=(vec &v, float t) { return v = v / t; }

//* scalar functions

[[nodiscard]] inline float operator*(vec v1, vec v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

[[nodiscard]] inline float norm(vec v) { return std::sqrt(v * v); }
[[nodiscard]] inline float dist(vec v1, vec v2) { return norm(v1 - v2); }
[[nodiscard]] inline float angle(vec v) { return std::atan2f(v.y, v.x); }

//* utility functions

[[nodiscard]] inline vec normalise(vec v) { return v / norm(v); }

[[nodiscard]] inline vec polar(float ang, float r = 1.0f) {
	return {r * std::cos(ang), r * std::sin(ang)};
}

[[nodiscard]] inline pair round(vec v) {
	return {std::lround(v.x), std::lround(v.y)};
}

[[nodiscard]] inline vec rotate(vec v, float ang) {
	return polar(angle(v) + ang, norm(v));
}
