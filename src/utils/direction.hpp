
#pragma once
#include <cstdint>
#include "vec.hpp"
#include <math.h>

enum class Direction : uint8_t {
	EAST,
	NORTH_EAST,
	NORTH,
	NORTH_WEST,
	WEST,
	SOUTH_WEST,
	SOUTH,
	SOUTH_EAST
};

const vec unitVectors[8] = {
    {1, 0},
	{1/sqrt(2), -1/sqrt(2)},
	{0, -1},
	{-1/sqrt(2), -1/sqrt(2)},
	{-1, 0},
	{-1/sqrt(2), 1/sqrt(2)},
	{0, 1},
	{1/sqrt(2), 1/sqrt(2)}
};