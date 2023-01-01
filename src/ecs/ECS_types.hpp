
#pragma once
#include "utils.hpp"

ENUM(ComponentId,
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
FORCE,
GRID,
INTERACTION,
NAME,
MONSTER_AI,
GATHER)

ENUM(SystemId,
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
PLAYER,
COLLIDER_DRAW,
GRID,
INTERACTION,
MONSTER_AI,
GATHER)

using Entity = std::uint32_t;
using Signature = std::bitset<ComponentId::count>;
using GridMap = std::unordered_map<pair, Entity>;
constexpr Entity MAX_ENTITIES = 500;

using Collisions = std::unordered_map<Entity,std::vector<Entity>>;
using gridMap = std::unordered_map<pair, Entity>;