#pragma once
#include <cassert>
#include <functional>
#include <ostream>

struct pair {
	int x, y;
	pair() : x(0), y(0) {}

	template <typename T1, typename T2>
	pair(T1 x, T2 y) : x(x), y(y) {}

	explicit operator bool() { return x != 0 || y != 0; }
};

template <>
struct std::hash<pair> {
	std::size_t operator()(::pair p) const { return std::hash<int>{}(p.x) ^ (std::hash<int>{}(p.y) << 1); }
};

//* comparison

[[nodiscard]] inline bool operator==(pair p1, pair p2) { return p1.x == p2.x && p1.y == p2.y; }
[[nodiscard]] inline bool operator!=(pair p1, pair p2) { return !(p1 == p2); }
[[nodiscard]] inline bool operator<(pair p1, pair p2) { return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y); }
[[nodiscard]] inline bool operator<=(pair p1, pair p2) { return p1 < p2 || p1 == p2; }
[[nodiscard]] inline bool operator>(pair p1, pair p2) { return p2 < p1; }
[[nodiscard]] inline bool operator>=(pair p1, pair p2) { return p2 < p1 || p1 == p2; }

//* addition

[[nodiscard]] inline pair operator+(pair p1, pair p2) { return {p1.x + p2.x, p1.y + p2.y}; }
inline pair operator+=(pair& p1, pair p2) { return p1 = p1 + p2; }

//* multiplication

[[nodiscard]] inline pair operator*(int n, pair p) { return {n * p.x, n * p.y}; }
[[nodiscard]] inline pair operator*(pair p, int n) { return n * p; }
inline pair operator*=(pair& p, int n) { return p = n * p; }

//* subtraction

[[nodiscard]] inline pair operator-(pair p) { return -1 * p; }
[[nodiscard]] inline pair operator-(pair p1, pair p2) { return p1 + (-p2); }
inline pair operator-=(pair p1, pair p2) { return p1 += (-p2); }

//* division

[[nodiscard]] inline pair operator/(pair p, int n) {
	assert(n != 0);
	return {p.x / n, p.y / n};
}
inline pair operator/=(pair& p, int n) { return p = p / n; }

//* mod

[[nodiscard]] inline pair operator%(pair p, int n) {
	assert(n != 0);
	return {p.x % n, p.y % n};
}
inline pair operator%=(pair& p, int n) { return p = p % n; }

//* scalar functions

[[nodiscard]] inline int operator*(pair p1, pair p2) { return p1.x * p2.x + p1.y * p2.y; }
[[nodiscard]] inline int norm_sq(pair p) { return p * p; }
[[nodiscard]] inline int dist_sq(pair p1, pair p2) { return norm_sq(p1 - p2); }
[[nodiscard]] inline int norm_tc(pair p) { return p.x + p.y; }
[[nodiscard]] inline int dist_tc(pair p1, pair p2) { return norm_tc(p1 - p2); }
