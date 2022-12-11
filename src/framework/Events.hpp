
#pragma once
#include <bitset>
#include "pair.hpp"

enum class InputEventId {
	NONE,

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

	MAX
};

struct InputEvent {
	InputEventId id;
	pair mousePosition;
};

enum class InputStateId {
	NONE,

	PRIMARY,
	SECONDARY,

	MOVE_EAST,
	MOVE_NORTH,
	MOVE_WEST,
	MOVE_SOUTH,

	INFO,
	ALTER,

	MAX
};

typedef std::bitset<size_t(InputStateId::MAX)> InputState;