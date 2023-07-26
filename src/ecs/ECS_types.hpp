
#pragma once
#include "utils.hpp"

ENUM(ComponentId,
POSITION,
SPRITE,
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
ITEM_KIND,
DAMAGE,
FORCE,
GRID,
INTERACTION,
NAME,
MONSTER_AI,
GATHER,
DEATH,
PARTICLE,
LIGHT,
SENSOR,
PROJECTILE,
LAUNCHER,
CHUNK,
AI,
AI_WANDER,
AI_MOVE,
AI_FLEE,
PORTAL)

ENUM(SystemId,
POSITION,
ENTITY_DRAW,
CREATURE_MOVEMENT,
CONTROLLER,
CREATURE_ANIMATION,
COLLISION,
ITEM_PICKUP,
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
GATHER,
DEATH,
INVENTORY_DEATH,
PARTICLE_EMIT,
CREATURE_PARTICLE,
HAND_RENDER,
CHUNK,
LIGHT,
SENSOR,
PROJECTILE,
AI,
AI_WANDER,
AI_MOVE,
AI_FLEE)

using Entity = uint;
using EntitySet = std::unordered_set<Entity>;
using Signature = std::bitset<ComponentId::count>;
using GridMap = std::unordered_map<pair, Entity>;
constexpr Entity MAX_ENTITIES = 100000;

using Collisions = std::unordered_map<Entity,std::vector<Entity>>;
using gridMap = std::unordered_map<pair, Entity>;