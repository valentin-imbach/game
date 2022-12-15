
#pragma once
#include <bitset>
#include "pair.hpp"
#include "enum.hpp"

ENUM(InputEventId,
PRIMARY,
SECONDARY,
ESCAPE,
INVENTORY,
THROW,
SELECT_1,
SELECT_2,
SELECT_3,
SELECT_4,
SELECT_5,
SELECT_6,
SELECT_7,
CONSOLE)

struct InputEvent {
	InputEventId::value id;
	pair mousePosition;
};

ENUM(InputStateId,
PRIMARY,
SECONDARY,
MOVE_EAST,
MOVE_NORTH,
MOVE_WEST,
MOVE_SOUTH,
INFO,
ALTER)

typedef std::bitset<InputStateId::count> InputState;