
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

const vec unitVectors[9] = {
	{0, 0},
	{1, 0},
	{1 / sqrt(2), -1 / sqrt(2)},
	{0, -1},
	{-1 / sqrt(2), -1 / sqrt(2)},
	{-1, 0},
	{-1 / sqrt(2), 1 / sqrt(2)},
	{0, 1},
	{1 / sqrt(2), 1 / sqrt(2)}};

const pair taxiSteps[9] = {
	{0, 0},
	{1, 0},
	{1, -1},
	{0, -1},
	{-1, -1},
	{-1, 0},
	{-1, 1},
	{0, 1},
	{1, 1}};