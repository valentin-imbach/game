#pragma once
#include "pair.hpp"
#include <cassert>
#include <cmath>

struct vec {
	float x, y;
	vec() : x(0), y(0) {}
	vec(pair p) : x(p.x), y(p.y) {}
	vec(float x, float y) : x(x), y(y) {}

	//* operations

	[[nodiscard]] inline vec operator*(float t) const {
		return vec(t * x, t * y);
	}

	[[nodiscard]] inline vec operator+(vec other) const {
		return vec(other.x + x, other.y + y);
	}
		
	inline vec operator+=(vec other) {
		return vec(x += other.x, y += other.y);
	}

	inline vec operator*=(float scalar) {
		return vec(x *= scalar, y *= scalar);
	}

	[[nodiscard]] inline vec operator-() const {
		return vec(-x, -y);
	}

	[[nodiscard]] inline vec operator-(vec other) const {
		return vec(x - other.x, y - other.y);
	}

	inline vec operator-=(vec other) {
		return vec(x -= other.x, y -= other.y);
	}

	[[nodiscard]] inline vec operator/(float scalar) const {
		assert(scalar != 0);
		return vec(x / scalar, y / scalar);
	}

	inline vec operator/=(float scalar) {
		assert(scalar != 0);
		return vec(x /= scalar, y /= scalar);
	}

	//* utility functions

	[[nodiscard]] static inline float dot(vec v1, vec v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	[[nodiscard]] static inline float norm(vec v) {
		return std::sqrt(dot(v, v));
	}

	[[nodiscard]] static inline float dist(vec v1, vec v2) {
		return norm(v1 - v2);
	}

	[[nodiscard]] static inline float angle(vec v) {
		return std::atan2f(-v.y, v.x);
	}

	[[nodiscard]] static inline vec normalise(vec v) {
		float d = norm(v);
		assert(d != 0);
		return v / d;
	}

	[[nodiscard]] static inline vec polar(float ang, float r = 1.0f) {
		return vec(std::cos(ang), std::sin(ang)) * r;
	}

	[[nodiscard]] static inline pair round(vec v) {
		return pair(std::lround(v.x), std::lround(v.y));
	}

	[[nodiscard]] static inline vec rotate(vec v, float ang) {
		return vec::polar(angle(v) + ang, norm(v));
	}

	[[nodiscard]] static inline pair ceil(vec v) {
		return pair(std::ceil(v.x), std::ceil(v.y));
	}

	[[nodiscard]] static inline pair floor(vec v) {
		return pair(std::floor(v.x), std::floor(v.y));
	}

};

[[nodiscard]] inline vec operator*(float t, vec v) {
	return v * t;
}

//* bounds checking

[[nodiscard]] inline bool isInside(vec p, vec position, vec size) {
	vec offset = p - position;
	return (abs(offset.x) <= size.x / 2 && abs(offset.y) <= size.y / 2);
}
