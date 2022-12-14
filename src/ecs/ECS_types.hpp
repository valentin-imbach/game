
#pragma once
#include "utils.hpp"

typedef std::uint32_t Entity;
constexpr Entity MAX_ENTITIES = 500;

enum class ComponentId : std::uint8_t {
	NONE,

	POSITION,
	SPRITE,
	CAMERA,
	CREATURE_STATE,
	MOVEMENT,
	DIRECTION,
	CONTROLLER,
	COLLIDER,
	ITEM,
	ANIMAL_AI,
	INVENTORY,
	HEALTH,
	PLAYER,
	RESOURCE,
	LOOT,
	TOOL,
	DAMAGE,

	MAX
};

using Signature = std::bitset<size_t(ComponentId::MAX)>;

enum class SystemId : std::uint8_t {
	NONE,

	ENTITY_DRAW,
	CREATURE_MOVEMENT,
	CONTROLLER,
	CAMERA,
	CREATURE_ANIMATION,
	COLLISION,
	ITEM_PICKUP,
	TILE,
	ANIMAL_AI,
	FORAGE,
	HEALTH,
	LOOT,
	DAMAGE,

	MAX
};

typedef std::unordered_map<pair, Entity> GridMap;
