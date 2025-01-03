
#pragma once
#include <bitset>
#include "vec.hpp"
#include "enum.hpp"

ENUM(InputEventId,
PRIMARY,
SECONDARY,

INVENTORY,
THROW,
CONSOLE,
DEBUG,
INSPECT,
MAP,
ZOOM_IN,
ZOOM_OUT,
QUIT,

SELECT_1,
SELECT_2,
SELECT_3,
SELECT_4,
SELECT_5,
SELECT_6,
SELECT_7,

ROTATE_RIGHT,
ROTATE_LEFT,

ESCAPE,
BACKSPACE,
RETURN,
TEXT,
UP,
DOWN,

STATE,
HOVER,
RESET)

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

struct InputEvent {
	InputEventId::value id;
	vec mousePosition;
	InputState state;
	std::string text;
};

