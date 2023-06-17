
#pragma once
#include "enum.hpp"
#include "pair.hpp"
#include "vec.hpp"
#include <cstdint>

ENUM(Direction,
	EAST,
	NORTH_EAST,
	NORTH,
	NORTH_WEST,
	WEST,
	SOUTH_WEST,
	SOUTH,
	SOUTH_EAST)

namespace Direction {

inline Direction::value from_vec(vec v) {
	return Direction::from_int(1 + int((vec::angle(v) + M_PI + M_PI / 8) / M_PI_4) % 8);
}

inline Direction::value rotate(Direction::value d, int n) {
	return Direction::from_int((d + n - 1) % 8 + 1);
}

const vec unit[9] = {
	vec(0, 0),
	vec(1, 0),
	vec(1 / std::sqrt(2), -1 / std::sqrt(2)),
	vec(0, -1),
	vec(-1 / std::sqrt(2), -1 / std::sqrt(2)),
	vec(-1, 0),
	vec(-1 / std::sqrt(2), 1 / std::sqrt(2)),
	vec(0, 1),
	vec(1 / std::sqrt(2), 1 / std::sqrt(2))};

const pair taxi[9] = {
	{0, 0},
	{1, 0},
	{1, -1},
	{0, -1},
	{-1, -1},
	{-1, 0},
	{-1, 1},
	{0, 1},
	{1, 1}};
	
} // namespace Direction
