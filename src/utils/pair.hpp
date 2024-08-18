#pragma once
#include <cassert>
#include <functional>
#include <ostream>

#include "json.hpp"

struct pair {
	int x = 0;
	int y = 0;
	
	pair() :  x(0), y(0) {};
	pair(int x, int y) : x(x), y(y) {}

	[[nodiscard]] explicit operator bool() {
		return (x != 0 || y != 0);
	}

	//* comparison

	[[nodiscard]] inline bool operator==(pair other) const {
		return (x == other.x && y == other.y);
	}

	[[nodiscard]] inline bool operator!=(pair other) const {
		return (x != other.x || y != other.y);
	}

	[[nodiscard]] inline bool operator<(pair other) const {
		return x < other.x || (x == other.x && y < other.y);
	}

	[[nodiscard]] inline bool operator<=(pair other) const {
		return x < other.x || (x == other.x && y <= other.y);
	}

	[[nodiscard]] inline bool operator>(pair other) const {
		return x > other.x || (x == other.x && y > other.y);
	}

	[[nodiscard]] inline bool operator>=(pair other) const {
		return x > other.x || (x == other.x && y >= other.y);
	}

	//* operations

	[[nodiscard]] inline pair operator+(pair other) const {
		return pair(x + other.x, y + other.y);
	}

	inline pair operator+=(pair other) {
		return pair(x += other.x, y += other.y);
	}

	[[nodiscard]] inline pair operator*(int scalar) const {
		return pair(scalar * x, scalar * y);
	}

	inline pair operator*=(int scalar) {
		return pair(x *= scalar, y *= scalar);
	}

	[[nodiscard]] inline pair operator-() const {
		return pair(-x, -y);
	}

	[[nodiscard]] inline pair operator-(pair other) const {
		return pair(x - other.x, y - other.y);
	}

	inline pair operator-=(pair other) {
		return pair(x -= other.x, y -= other.y);
	}

	[[nodiscard]] inline pair operator/(int scalar) const {
		assert(scalar != 0);
		return pair(x / scalar, y / scalar);
	}

	inline pair operator/=(int scalar) {
		assert(scalar != 0);
		return pair(x /= scalar, y /= scalar);
	}

	[[nodiscard]] inline pair operator%(int scalar) const {
		assert(scalar != 0);
		return pair(x % scalar, y % scalar);
	}

	[[nodiscard]] inline pair operator%=(int scalar) {
		assert(scalar != 0);
		return pair(x %= scalar, y %= scalar);
	}

	//* utility functions

	[[nodiscard]] inline static int dot(pair p1, pair p2) {
		return p1.x * p2.x + p1.y * p2.y;
	}

	[[nodiscard]] inline static int snorm(pair p) {
		return dot(p, p);
	}

	[[nodiscard]] inline static int sdist(pair p1, pair p2) {
		return snorm(p1 - p2);
	}

	[[nodiscard]] inline static int tnorm(pair p) {
		return p.x + p.y;
	}

	[[nodiscard]] inline static int tdist(pair p1, pair p2) {
		return tnorm(p1 - p2);
	}

	[[nodiscard]] inline static bool inside(pair p, pair position, pair size, bool centered = true) {
		pair offset = p - (position + size/2);
		if (centered) offset = p - position;
		return (std::abs(offset.x) <= size.x / 2 && std::abs(offset.y) <= size.y / 2);
	}

	[[nodiscard]] inline static int avg(pair p) {
		return (p.x + p.y) / 2;
	}

	[[nodiscard]] inline static int diff(pair p) {
		return std::abs(p.y - p.x);
	}

};

[[nodiscard]] inline pair operator*(int n, pair p) {
	return p * n;
}

template <>
struct std::hash<pair> {
	uint operator()(::pair p) const {
		return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 1);
	}
};

inline pair parsePair(json::Value value) {
	assert(value.is<json::Array>() && value.size() == 2);
	return pair(int(value[0]), int(value[1]));
}