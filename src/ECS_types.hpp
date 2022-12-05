
#pragma once
#include "utils/utils.hpp"

typedef std::uint32_t Entity;
constexpr Entity MAX_ENTITIES = 100;

enum class ComponentId : std::uint8_t {
	NONE,

	POSITION,
	SPRITE,
	CAMERA,

	MAX
};

using Signature = std::bitset<size_t(ComponentId::MAX)>;

enum class SystemId : std::uint8_t {
	NONE,

	SPRITE,
	GRAVITY,

	MAX
};
