
#pragma once
#include <cstdint>
#include "pair.hpp"
#include "vec.hpp"
#include "enum.hpp"

ENUM(Direction,
EAST,
NORTH_EAST,
NORTH,
NORTH_WEST,
WEST,
SOUTH_WEST,
SOUTH,
SOUTH_EAST)

const vec unitVectors[8] = {
    {1, 0},
    {1 / sqrt(2), -1 / sqrt(2)},
    {0, -1},
    {-1 / sqrt(2), -1 / sqrt(2)},
    {-1, 0},
    {-1 / sqrt(2), 1 / sqrt(2)},
    {0, 1},
    {1 / sqrt(2), 1 / sqrt(2)}
};

const pair taxiSteps[8] = {
    {1, 0},
    {1, -1},
    {0, -1},
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, 1},
    {1, 1}
};